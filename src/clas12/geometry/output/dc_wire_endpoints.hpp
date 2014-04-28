#ifndef __CLAS12_GEOMETRY_OUTPUT_DC_WIRE_ENDPOINTS_HPP__
#define __CLAS12_GEOMETRY_OUTPUT_DC_WIRE_ENDPOINTS_HPP__

#include <memory>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

#include "geometry/euclid_vector.hpp"
#include "geometry/line_segment.hpp"

#include "clas12/geometry/drift_chamber.hpp"

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

using ::geometry::line_segment;
using ::geometry::euclid_vector;

using clas12::geometry::DriftChamber;

void dc_wire_endpoints_xml(xml_document& doc, const DriftChamber& dc, const string& coordsys="sector", const string& units="cm")
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

    xml_node dc_node = geom_node.child("drift_chamber");
    if (!dc_node)
    {
        dc_node = geom_node.append_child("drift_chamber");
    }

    xml_node wire_node = dc_node.child("wire_endpoints");
    if (!wire_node)
    {
        wire_node = dc_node.append_child("wire_endpoints");
        wire_node.append_attribute("length_units");
        wire_node.append_attribute("coordinate_system");
    }
    wire_node.attribute("length_units") = units.c_str();
    wire_node.attribute("coordinate_system") = coordsys.c_str();

    for (int sec=0; sec<dc.sectors().size(); sec++)
    {
        const drift_chamber::Sector& dc_sector = dc.sector(sec);

        for (int reg=0; reg<dc_sector.regions().size(); reg++)
        {
            const drift_chamber::Region& dc_region = dc_sector.region(reg);

            for (int slyr=0; slyr<dc_region.superlayers().size(); slyr++)
            {
                const drift_chamber::Superlayer& dc_superlayer = dc_region.superlayer(slyr);

                for (int lyr=0; lyr<dc_superlayer.senselayers().size(); lyr++)
                {
                    const drift_chamber::Senselayer& dc_senselayer = dc_superlayer.senselayer(lyr);

                    xml_node layer_node = wire_node.append_child("layer");
                    layer_node.append_attribute("sector") = sec;
                    layer_node.append_attribute("region") = reg;
                    layer_node.append_attribute("superlayer") = slyr;
                    layer_node.append_attribute("senselayer") = lyr;

                    xml_node left_node = layer_node.append_child("left");
                    vector<xml_node> endpoint_nodes;
                    endpoint_nodes.push_back(left_node.append_child("x"));
                    endpoint_nodes.push_back(left_node.append_child("y"));
                    endpoint_nodes.push_back(left_node.append_child("z"));

                    xml_node right_node = layer_node.append_child("right");
                    endpoint_nodes.push_back(right_node.append_child("x"));
                    endpoint_nodes.push_back(right_node.append_child("y"));
                    endpoint_nodes.push_back(right_node.append_child("z"));

                    vector<stringstream> endpoints(6);
                    for (line_segment<double,3> w : dc_senselayer.wires(coord))
                    {
                        euclid_vector<double,3> endpt = w.end_point();
                        endpoints[0] << " " << w.begin_point().x() * lconv;
                        endpoints[1] << " " << w.begin_point().y() * lconv;
                        endpoints[2] << " " << w.begin_point().z() * lconv;
                        endpoints[3] << " " << endpt.x() * lconv;
                        endpoints[4] << " " << endpt.y() * lconv;
                        endpoints[5] << " " << endpt.z() * lconv;
                    }

                    for (int i=0; i<endpoints.size(); i++)
                    {
                        endpoint_nodes[i].append_child(pugi::node_pcdata).set_value(
                            endpoints[i].str().erase(0,1).c_str());
                    }
                }

                for (int lyr=0; lyr<dc_superlayer.guardlayers().size(); lyr++)
                {
                    const drift_chamber::Guardlayer& dc_guardlayer = dc_superlayer.guardlayer(lyr);

                    xml_node layer_node = wire_node.append_child("layer");
                    layer_node.append_attribute("sector") = sec;
                    layer_node.append_attribute("region") = reg;
                    layer_node.append_attribute("superlayer") = slyr;
                    layer_node.append_attribute("guardlayer") = lyr;

                    xml_node left_node = layer_node.append_child("left");
                    vector<xml_node> endpoint_nodes;
                    endpoint_nodes.push_back(left_node.append_child("x"));
                    endpoint_nodes.push_back(left_node.append_child("y"));
                    endpoint_nodes.push_back(left_node.append_child("z"));

                    xml_node right_node = layer_node.append_child("right");
                    endpoint_nodes.push_back(right_node.append_child("x"));
                    endpoint_nodes.push_back(right_node.append_child("y"));
                    endpoint_nodes.push_back(right_node.append_child("z"));

                    vector<stringstream> endpoints(6);
                    for (line_segment<double,3> w : dc_guardlayer.wires(coord))
                    {
                        euclid_vector<double,3> endpt = w.end_point();
                        endpoints[0] << " " << w.begin_point().x() * lconv;
                        endpoints[1] << " " << w.begin_point().y() * lconv;
                        endpoints[2] << " " << w.begin_point().z() * lconv;
                        endpoints[3] << " " << endpt.x() * lconv;
                        endpoints[4] << " " << endpt.y() * lconv;
                        endpoints[5] << " " << endpt.z() * lconv;
                    }

                    for (int i=0; i<endpoints.size(); i++)
                    {
                        endpoint_nodes[i].append_child(pugi::node_pcdata).set_value(
                            endpoints[i].str().erase(0,1).c_str());
                    }
                }
            }
        }
    }
}

} // namespace clas12::geometry::output
} // namespace clas12::geometry
} // namespace clas12

#endif // __CLAS12_GEOMETRY_OUTPUT_DC_WIRE_ENDPOINTS_HPP__
