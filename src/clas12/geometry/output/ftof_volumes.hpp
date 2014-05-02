#ifndef CLAS12_GEOMETRY_OUTPUT_FTOF_SECTOR_VOLUMES_HPP
#define CLAS12_GEOMETRY_OUTPUT_FTOF_SECTOR_VOLUMES_HPP

#include <iostream>
using std::clog;
using std::endl;

#include <algorithm>
#include <cstddef>
#include <map>
#include <sstream>
#include <string>
#include <vector>

#include "pugixml.hpp"

#include "geometry.hpp"
#include "geometry/constants.hpp"

#include "clas12/geometry/forward_tof.hpp"

namespace clas12
{
namespace geometry
{
namespace output
{

using std::map;
using std::string;
using std::copy;
using std::inserter;

using pugi::xml_document;
using pugi::xml_node;
using pugi::node_pcdata;

typedef map<string, map<string,string> > volmap_t;

/**
 * \brief generate the volumes of a FTOF Sector for input into gemc/geant4
 *
 * The numbers for the mother volumes are calculated following Geant4's TRD constructor:

 *     pDx1    Half-length along x at the z surface positioned at -DZ
 *     pDx2    Half-length along x at the z surface positioned at +DZ
 *     pDy     Half-length along y
 *     pDz     Half-length along z
 *
 * \return map of map of strings: ret[volume_name][param_name] = value
 **/
 // 4 mm gap between panel's mother volume and daughter volumes
static const double mothergap = 0.4; //cm


volmap_t ftof_volumes_map(const ForwardTOF& ftof)
{
    using namespace std;
    using namespace ::geometry;

    static const double double_tolerance = 1.e-8;

    // everything in clas12/geometry is in radians, but gemc
    // seems to want degrees.
    static const double rad2deg = 180. / cons::pi<double>();


    double dx1;
    double dx2;
    double dy;
    double dz;

    volmap_t vols;

    for (size_t sec=0; sec<ftof.sectors().size(); sec++)
    {
        const forward_tof::Sector& sector = ftof.sector(sec);

        stringstream sector_name_ss;
        sector_name_ss << "sec" << sector.index()+1;
        string sector_name = sector_name_ss.str();

        stringstream sector_desc;
        sector_desc << "Forward Time Of Flight"
                    << " Sector " << sector.index()+1;

        for (size_t pan=0; pan<sector.panels().size(); pan++)
        {
            const forward_tof::Panel& panel = sector.panel(pan);

            stringstream panel_name_ss;
            panel_name_ss << "sec" << sector.index()+1
                          << "_pan" << sector.panel_name(pan);
            string panel_name = panel_name_ss.str();

            stringstream panel_desc;
            panel_desc << "Forward Time Of Flight"
                       << " Sector " << sector.index()+1
                       << " Panel " << sector.panel_name(pan);

            dx1 = 0.5 * panel.paddle_length(0) + mothergap;
            dx2 = 0.5 * panel.paddle_length(-1) + mothergap
                + 0.5 * (panel.paddle_length(-1) - panel.paddle_length(-2));
            dy  = 0.5 * panel.paddle_thickness() + mothergap;
            dz  = 0.5 * panel.radial_extent() + mothergap;

            //panel's center point in CLAS coordinate system
            euclid_vector<> panel_center = panel.center(COORDSYS::CLAS);

            stringstream panel_pos;
            panel_pos << panel_center.x() << "*cm "
                      << panel_center.y() << "*cm "
                      << panel_center.z() << "*cm";

            stringstream panel_rot;
            panel_rot << "ordered: zxy"
                      << " " << -90. - 60.*sector.index() << "*deg"
                      << " " << -90. - panel.thtilt()*rad2deg << "*deg"
                      << " " << 0 << "*deg";

            stringstream panel_dim;
            panel_dim << dx1 << "*cm "
                      << dx2 << "*cm "
                      << dy << "*cm "
                      << dy << "*cm "
                      << dz << "*cm ";

            // The Panel mother volume
            vols[panel_name] = {
                {"mother", "root"},
                {"description", panel_desc.str()},
                {"pos", panel_pos.str()},
                {"rotation",  panel_rot.str()},
                {"color", "ff11aa5"},
                {"type", "Trd"},
                {"dimensions", panel_dim.str()},
                {"material", "G4_AIR"},
                {"mfield", "no"},
                {"ncopy", "1"},
                {"pMany", "1"},
                {"exist", "1"},
                {"visible", "0"},
                {"style", "0"},
                {"sensitivity", "no"},
                {"hit_type", ""},
                {"identifiers", ""}
            };

            for (int pad = 0; pad<panel.paddles().size(); pad++)
            {

                /*
                generating parameters for the paddle volumes following
                the BOX constructor:
                    pDx     Half-length along the x-axis
                    pDy     Half-length along the y-axis
                    pDz     Half-length along the z-axis
                */

                double dx = panel.paddle_length(pad)/2.0;
                double dy = panel.paddle_thickness()/2.0;
                double dz = panel.paddle_width()/2.0;

                // posz is the GEANT z position of each paddle
                // (corresponding to x in sector coords)
                double posz =
                    (panel.paddle_center_x(pad) - panel_center.x())
                    / cos(panel.thtilt());

                stringstream paddle_name_ss;
                paddle_name_ss << "sec" << sector.index()+1
                               << "_pan" << sector.panel_name(pan)
                               << "_pad" << pad+1;
                string paddle_name = paddle_name_ss.str();

                stringstream paddle_desc;
                paddle_desc << "Forward Time Of Flight"
                            << " Sector " << sector.index()+1
                            << " Panel " << sector.panel_name(pan)
                            << " Paddle " << pad+1;

                //paddle's position relative to the panel (mother volume)
                euclid_vector<> paddle_position = {0,0,posz};

                stringstream paddle_pos;
                paddle_pos << paddle_position.x() << "*cm "
                           << paddle_position.y() << "*cm "
                           << paddle_position.z() << "*cm";

                stringstream paddle_rot;
                paddle_rot << "0*deg 0*deg 0*deg";

                stringstream paddle_dim;
                paddle_dim << dx << "*cm "<< dy << "*cm "<< dz << "*cm ";

                stringstream paddle_ids;
                stringstream paddle_sens;

                paddle_ids    << "sector ncopy 0 paddle manual " << pad+1 ;
                paddle_sens   << "FTOF_"<<sector.panel_name(pan);

                // The paddle volume
                vols[paddle_name] = {
                    {"mother", panel_name},
                    {"description", paddle_desc.str()},
                    {"pos", paddle_pos.str()},
                    {"rotation", paddle_rot.str()},
                    {"color", "ff11aa"},
                    {"type", "Box"},
                    {"dimensions", paddle_dim.str()},
                    {"material", "scintillator"},
                    {"mfield", "no"},
                    {"ncopy", "1"},
                    {"pMany", "1"},
                    {"exist", "1"},
                    {"visible", "1"},
                    {"style", "1"},
                    {"sensitivity", paddle_sens.str()},
                    {"hit_type", paddle_sens.str()},
                    {"identifiers", paddle_ids.str()}
                };

            } // loop over paddles

        }  // loop over panels

    } // loop over sectors

    return vols;
}

void ftof_volumes_xml(xml_document& doc, const ForwardTOF& ftof)
{
    // start building up the XML document
    xml_node geom_node = doc.child("geometry");
    if (!geom_node)
    {
        geom_node = doc.append_child("geometry");
    }
    xml_node dc_node = geom_node.child("forward_tof");
    if (!dc_node)
    {
        dc_node = geom_node.append_child("forward_tof");
    }
    xml_node vol_node = dc_node.child("volumes");
    if (!vol_node)
    {
        vol_node = dc_node.append_child("volumes");
    }

    for (auto k1 : ftof_volumes_map(ftof))
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

#endif // CLAS12_GEOMETRY_OUTPUT_FTOF_SECTOR_VOLUMES_HPP
