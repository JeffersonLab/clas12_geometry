#ifndef CLAS12_GEOMETRY_OUTPUT_DC_SECTOR_VOLUMES_HPP
#define CLAS12_GEOMETRY_OUTPUT_DC_SECTOR_VOLUMES_HPP

#include <iostream>
using std::clog;
using std::endl;

#include <cstddef>
#include <map>
#include <sstream>
#include <string>
#include <vector>

#include "pugixml.hpp"

#include "geometry.hpp"
#include "geometry/constants.hpp"

#include "clas12/geometry/drift_chamber.hpp"

namespace clas12
{
namespace geometry
{
namespace output
{

using std::map;
using std::string;

using pugi::xml_document;
using pugi::xml_node;
using pugi::node_pcdata;

typedef map<string, map<string,string> > volmap_t;

/**
 * \brief generate the volumes of a DC Sector for input into gemc/geant4
 *
 * The numbers calculated are following Geant4's G4Trap constructor:
 *     pDz     Half-length along the z-axis
 *     pTheta  Polar angle of the line joining the centres of the faces
 *             at -/+pDz
 *     pPhi    Azimuthal angle of the line joing the centre of the face
 *             at -pDz to the centre of the face at +pDz
 *     pDy1    Half-length along y of the face at -pDz
 *     pDx1    Half-length along x of the side at y=-pDy1 of the face at -pDz
 *     pDx2    Half-length along x of the side at y=+pDy1 of the face at -pDz
 *     pAlp1   Angle with respect to the y axis from the centre of the
 *             side at y=-pDy1 to the centre at y=+pDy1 of the face at -pDz
 *
 *     pDy2    Half-length along y of the face at +pDz
 *     pDx3    Half-length along x of the side at y=-pDy2 of the face at +pDz
 *     pDx4    Half-length along x of the side at y=+pDy2 of the face at +pDz
 *     pAlp2   Angle with respect to the y axis from the centre of the
 *             side at y=-pDy2 to the centre at y=+pDy2 of the face at +pDz
 *
 * \return map of map of strings: ret[volume_name][param_name] = value
 **/
volmap_t dc_sector_volumes_map(const drift_chamber::Sector& sector)
{
    using namespace std;
    using namespace ::geometry;

    // everything in clas12/geometry is in radians, but gemc
    // seems to want degrees.
    static const double rad2deg = 180. / cons::pi<double>();

    double dz;
    double theta;
    double phi;
    double dy1;
    double dx1;
    double dx2;
    double alp1;
    double dy2;
    double dx3;
    double dx4;
    double alp2;

    // vols[name][var] = val
    volmap_t vols;

    stringstream sector_name_ss;
    sector_name_ss << "sector" << sector.index()+1;
    string sector_name = sector_name_ss.str();

    stringstream sector_desc;
    sector_desc << "Drift Chamber"
                << " Sector " << sector.index()+1;

    for (size_t reg=0; reg<sector.regions().size(); reg++)
    {
        const drift_chamber::Region& region = sector.region(reg);

        stringstream region_name_ss;
        region_name_ss << "R" << reg+1 << "_S" << sector.index()+1;
        string region_name = region_name_ss.str();

        stringstream region_desc;
        sector_desc << "Drift Chamber"
                    << " Sector " << sector.index()+1
                    << " Region " << reg+1;

        // end plates of this region
        // used in sense-layer volume calculations.
        plane<> lplate = region.left_end_plate();
        plane<> rplate = region.right_end_plate();

        // first and last guard wire endpoints
        euclid_vector<> guardwire0_endpoint = \
            region.superlayer( 0).guardlayer( 0).wire( 0).end_point();
        euclid_vector<> guardwire1_endpoint = \
            region.superlayer(-1).guardlayer(-1).wire(-1).end_point();

        // region center-point in sector coordinates
        euclid_vector<> region_center = region.center();

        // x and y are reversed for gemc's coordinate system
        dz    = 0.5 * region.thickness();
        theta = - region.thtilt();
        phi   = 0.5 * cons::pi<double>();
        dy1   = 0.5 * (guardwire1_endpoint.x() - guardwire0_endpoint.x())
                / cos(region.thtilt());
        dx1   = guardwire0_endpoint.y();
        dx2   = guardwire1_endpoint.y();
        alp1  = 0.;
        dy2   = dy1;
        dx3   = dx1;
        dx4   = dx2;
        alp2  = alp1;

        // x and y are reversed for gemc's coordinate system
        stringstream region_pos;
        region_pos << region_center.y() << "*cm "
                   << region_center.x() << "*cm "
                   << region_center.z() << "*cm";

        stringstream region_rot;
        region_rot << "ordered: zxy "
                  << " " <<  90. + region.thtilt()*rad2deg << "*deg"
                  << " " << -90. - 60.*sector.index() << "*deg"
                  << " " << 0 << "*deg";

        stringstream region_dim;
        region_dim << dz << "*cm "
                   << theta * rad2deg << "*deg "
                   << phi * rad2deg << "*deg "
                   << dy1 << "*cm "
                   << dx1 << "*cm "
                   << dx2 << "*cm "
                   << alp1 * rad2deg << "*deg "
                   << dy2 << "*cm "
                   << dx3 << "*cm "
                   << dx4 << "*cm "
                   << alp2 * rad2deg << "*deg";

        // The Region mother volume
        vols[region_name] = {
            {"mother", "root"},
            {"description", region_desc.str()},
            {"pos", region_pos.str()},
            {"rotation", region_rot.str()},
            {"color", "aa0000"},
            {"type", "G4Trap"},
            {"dimensions", region_dim.str()},
            {"material", "DCgas"},
            {"mfield", "no"},
            {"ncopy", "1"},
            {"pMany", "1"},
            {"exist", "1"},
            {"visible", "1"},
            {"style", "0"},
            {"sensitivity", "no"},
            {"hit_type", ""},
            {"identifiers", ""}
        };

        for(size_t slyr=0; slyr<region.superlayers().size(); slyr++)
        {
            const drift_chamber::Superlayer& superlayer = region.superlayer(slyr);

            for (size_t lyr=0; lyr<2 /*superlayer.senselayers().size()*/; lyr++)
            {

/// begin testing senselayer methods ///////////////////////////////////
const drift_chamber::Senselayer& senselayer = superlayer.senselayer(lyr);

/*
generating the trapezoid parameters for this senselayer
following the G4Trap constructor:
    pDz     Half-length along the z-axis
    pTheta  Polar angle of the line joining the centres of the faces
            at -/+pDz
    pPhi    Azimuthal angle of the line joing the centre of the face at
            -pDz to the centre of the face at +pDz
    pDy1    Half-length along y of the face at -pDz
    pDx1    Half-length along x of the side at y=-pDy1 of the face at -pDz
    pDx2    Half-length along x of the side at y=+pDy1 of the face at -pDz
    pAlp1   Angle with respect to the y axis from the centre of the side
            at y=-pDy1 to the centre at y=+pDy1 of the face at -pDz

    pDy2    Half-length along y of the face at +pDz
    pDx3    Half-length along x of the side at y=-pDy2 of the face at +pDz
    pDx4    Half-length along x of the side at y=+pDy2 of the face at +pDz
    pAlp2   Angle with respect to the y axis from the centre of the side
            at y=-pDy2 to the centre at y=+pDy2 of the face at +pDz
*/

// all done in sector coordinate system. ///////////////////////////////


// 100 um gap between layers (to avoid G4 volume overlap)
static const double microgap = 0.01;

double hflyrthk = 0.5 * superlayer.layer_thickness();
euclid_vector<double,3> half_lyr_thickness{
    hflyrthk * sin(region.thtilt()),
    0.,
    hflyrthk * cos(region.thtilt()) };

// volume edges as infinitely-extending lines
// The first two edges are the wire-lines displaced by
// half a layer-thickness in the direction of (0,0,0).
// The last two are displaced away from the origin.
line<double,3> edge00_line{
    senselayer.wire_mid( 0) - half_lyr_thickness,
    superlayer.wire_direction() };
line<double,3> edge01_line{
    senselayer.wire_mid(-1) - half_lyr_thickness,
    superlayer.wire_direction() };
line<double,3> edge10_line{
    senselayer.wire_mid( 0) + half_lyr_thickness,
    superlayer.wire_direction() };
line<double,3> edge11_line{
    senselayer.wire_mid(-1) + half_lyr_thickness,
    superlayer.wire_direction() };

// get the intersection and create line segment from one
// point to the other. These are the same lines as above
// but with endpoints at the left and right end-planes.
line_segment<double,3> edge00{
    intersection(edge00_line, lplate),
    intersection(edge00_line, rplate) };
line_segment<double,3> edge01{
    intersection(edge01_line, lplate),
    intersection(edge01_line, rplate) };
line_segment<double,3> edge10{
    intersection(edge10_line, lplate),
    intersection(edge10_line, rplate) };
line_segment<double,3> edge11{
    intersection(edge11_line, lplate),
    intersection(edge11_line, rplate) };

// Layer closer to the origin:
// p0 = projection of the 1st edge's midpoint onto the
//      line representing the longer edge on the same layer.
// d00 = direction from the 1st edges's midpoint to p0
// d01 = direction from the 1st to the 2nd edges's midpoint
euclid_vector<double,3> p0 = projection(edge00.mid_point(), edge01_line);
direction_vector<double,3> d00{p0 - edge00.mid_point()};
direction_vector<double,3> d01{edge01.mid_point() - edge00.mid_point()};

// Layer farther away from the origin:
// p1 = projection of the 1st edge's midpoint onto the
//      line representing the longer edge on the same layer.
euclid_vector<double,3> p1 = projection(edge10.mid_point(), edge11_line);

// get the sign of the alp angle for the trapezoid
double sign_of_alp1 = 1;
if ((edge01.mid_point().y() - p0.y()) < 0.)
{
    sign_of_alp1 = -1;
}

dz    = hflyrthk - microgap;
theta = - region.thtilt();
phi   = 0.5 * cons::pi<double>();
dy1   = 0.5 * line_segment<double,3>(edge00.mid_point(), p0).length();
dx1   = 0.5 * edge00.length();
dx2   = 0.5 * edge01.length();
alp1  = sign_of_alp1 * d00.angle(d01);
dy2   = 0.5 * line_segment<double,3>(edge10.mid_point(), p1).length();
dx3   = 0.5 * edge10.length();
dx4   = 0.5 * edge11.length();
alp2  = alp1;

stringstream layer_name_ss;
layer_name_ss << "L" << lyr+1 << "_SL" << slyr+1 << "_R" << reg+1 << "_S" << sector.index()+1;
string layer_name = layer_name_ss.str();

stringstream layer_desc;
layer_desc << "Drift Chamber"
           << " Sector " << sector.index()+1
           << " Region " << reg+1
           << " Superlayer " << slyr+1
           << " Senselayer " << lyr+1;

// d = position of layer volume relative to the region (mother volume)
euclid_vector<> d = senselayer.center() - region_center;

// rotate about the y-axis in sector coordinates by the region's tilt
// this is done because the trapezoids are defined by the edges
// and the whole volume is then rotated to get the final position
d = { cos(region.thtilt())*d.x() - sin(region.thtilt())*d.z(),
      d.y(),
      cos(region.thtilt())*d.z() + sin(region.thtilt())*d.x() };

// x and y are reversed for gemc's coordinate system
stringstream layer_pos;
layer_pos << d.y() << "*cm " << d.x() << "*cm " << d.z() << "*cm";

stringstream layer_rot;
layer_rot << "0*deg 0*deg " << superlayer.thster() * rad2deg << "*deg";

stringstream layer_dim;
layer_dim << dz << "*cm "
          << theta * rad2deg << "*deg "
          << phi * rad2deg << "*deg "
          << dy1 << "*cm "
          << dx1 << "*cm "
          << dx2 << "*cm "
          << alp1 * rad2deg << "*deg "
          << dy2 << "*cm "
          << dx3 << "*cm "
          << dx4 << "*cm "
          << alp2 * rad2deg << "*deg";

stringstream layer_ids;
layer_ids << "sector ncopy 0 superlayer manual " << slyr+1 << " layer manual " << lyr+1 << " wire manual 1";

// The (Sense)Layer volume
vols[layer_name] = {
    {"mother", region_name},
    {"description", layer_desc.str()},
    {"pos", layer_pos.str()},
    {"rotation", layer_rot.str()},
    {"color", "66aadd"},
    {"type", "G4Trap"},
    {"dimensions", layer_dim.str()},
    {"material", "DCgas"},
    {"mfield", "no"},
    {"ncopy", "1"},
    {"pMany", "1"},
    {"exist", "1"},
    {"visible", "1"},
    {"style", "1"},
    {"sensitivity", "DC"},
    {"hit_type", "DC"},
    {"identifiers", layer_ids.str()}
};

/// end testing senselayer methods /////////////////////////////////////
            }
        }
    }

    return vols;
}


volmap_t dc_volumes_map(const DriftChamber& dc)
{
    volmap_t vmap;

    for (const unique_ptr<drift_chamber::Sector>& sector : dc.sectors())
    {
        volmap_t secmap = dc_sector_volumes_map(*sector);
        vmap.insert(secmap.begin(), secmap.end());
    }

    return vmap;
}

void dc_volumes_xml(xml_document& doc, const DriftChamber& dc)
{
    // start building up the XML document
    xml_node geom_node = doc.child("geometry");
    if (!geom_node)
    {
        geom_node = doc.append_child("geometry");
    }
    xml_node dc_node = geom_node.child("drift_chamber");
    if (!dc_node)
    {
        dc_node = geom_node.append_child("drift_chamber");
    }
    xml_node vol_node = dc_node.child("volumes");
    if (!vol_node)
    {
        vol_node = dc_node.append_child("volumes");
    }

    for (auto k1 : dc_volumes_map(dc))
    {
        xml_node n1 = vol_node.append_child(k1.first.c_str());

        for (auto k2 : k1.second)
        {
            xml_node n2 = n1.append_child(k2.first.c_str());
            n2.append_child(node_pcdata).set_value(k2.second.c_str());
        }
    }
}

} // namespace clas12::geometry::output
} // namespace clas12::geometry
} // namespace clas12

#endif // CLAS12_GEOMETRY_OUTPUT_DC_SECTOR_VOLUMES_HPP
