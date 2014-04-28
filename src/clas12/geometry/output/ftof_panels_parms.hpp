#ifndef __CLAS12_GEOMETRY_OUTPUT_FTOF_PANELS_PARMS_HPP__
#define __CLAS12_GEOMETRY_OUTPUT_FTOF_PANELS_PARMS_HPP__

#include <memory>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

#include "geometry/plane.hpp"
#include "geometry/euclid_vector.hpp"
#include "geometry/direction_vector.hpp"

#include "clas12/geometry.hpp"

#include "detail/length_conversion.hpp"

namespace clas12
{
namespace geometry
{
namespace output
{

using std::stringstream;
using std::string;
using std::vector;

using std::runtime_error;

using pugi::xml_document;
using pugi::xml_node;

using ::geometry::plane;
using ::geometry::euclid_vector;
using ::geometry::direction_vector;

using clas12::geometry::ForwardTOF;

void ftof_panels_parms_xml(xml_document& doc, const ForwardTOF& ftof, const string& coordsys="clas", const string& units="cm")
{
    double lconv = length_conversion(units);

    if (coordsys != "sector" && coordsys != "clas")
    {
        throw runtime_error(string("can not generate data in ") + coordsys + " coordinates");
    }
    coordsys_t coord = str2coord(coordsys);

    // start building up the XML document
    xml_node geom_node = doc.child("geometry");
    if (!geom_node)
    {
        geom_node = doc.append_child("geometry");
    }

    xml_node ftof_node = geom_node.child("forward_tof");
    if (!ftof_node)
    {
        ftof_node = geom_node.append_child("forward_tof");
    }

    xml_node panels_node = ftof_node.child("panels_parms");
    if (!panels_node)
    {
        panels_node = ftof_node.append_child("panels_parms");
        panels_node.append_attribute("length_units");
        panels_node.append_attribute("coordinate_system");
    }

    panels_node.attribute("length_units") = units.c_str();
    panels_node.attribute("coordinate_system") = coordsys.c_str();

    for (int sec=0; sec<ftof.sectors().size(); sec++)
    {
        const forward_tof::Sector& ftof_sector = ftof.sector(sec);

        for (int pan=0; pan<ftof_sector.panels().size(); pan++)
        {
            const forward_tof::Panel& panel = ftof_sector.panel(pan);

            xml_node panel_node = panels_node.append_child("panel");
            panel_node.append_attribute("sector") = sec;
            panel_node.append_attribute("panel") = ftof_sector.panel_name(pan).c_str();
            panel_node.append_attribute("npaddles") = int(panel.npaddles());
            panel_node.append_attribute("dist2tgt") = panel.dist2tgt() * lconv;

            direction_vector<double,3> panel_norm = panel.panel_normal(coord);
            panel_node.append_attribute("norm_phi") = panel_norm.phi();
            panel_node.append_attribute("norm_theta") = panel_norm.theta();

            direction_vector<double,3> paddle_dir = panel.paddle_direction(coord);
            panel_node.append_attribute("paddle_phi") = paddle_dir.phi();
            panel_node.append_attribute("paddle_theta") = paddle_dir.theta();

            panel_node.append_attribute("paddle_width") = panel.paddle_width() * lconv;
            panel_node.append_attribute("paddle_thickness") = panel.paddle_thickness() * lconv;

            euclid_vector<double,3> paddle_extent = panel.paddle_extent(COORDSYS::SECTOR);
            panel_node.append_attribute("paddle_extent_x") = paddle_extent.x() * lconv;
            panel_node.append_attribute("paddle_extent_z") = paddle_extent.z() * lconv;

            xml_node center_node = panel_node.append_child("paddle_centers");
            vector<xml_node> paddle_centers_node;
            paddle_centers_node.push_back(center_node.append_child("x"));
            paddle_centers_node.push_back(center_node.append_child("y"));
            paddle_centers_node.push_back(center_node.append_child("z"));

            vector<stringstream> centers(3);
            for (auto cntr : panel.paddle_centers(coord))
            {
                centers[0] << " " << cntr.x() * lconv;
                centers[1] << " " << cntr.y() * lconv;
                centers[2] << " " << cntr.z() * lconv;
            }
            for (int i=0; i<paddle_centers_node.size(); i++)
            {
                paddle_centers_node[i].append_child(pugi::node_pcdata).set_value(
                    centers[i].str().erase(0,1).c_str());
            }

            xml_node length_node = panel_node.append_child("paddle_lengths");
            stringstream lengths;
            for (auto lngth : panel.paddle_lengths())
            {
                lengths << " " << lngth;
            }
            length_node.append_child(pugi::node_pcdata).set_value(
                lengths.str().erase(0,1).c_str());
        }
    }
}

} // namespace clas12::geometry::output
} // namespace clas12::geometry
} // namespace clas12

#endif // __CLAS12_GEOMETRY_OUTPUT_FTOF_PANELS_PARMS_HPP__
