#ifndef __CLAS12_GEOMETRY_OUTPUT_BST_STRIP_ENDPOINTS_HPP__
#define __CLAS12_GEOMETRY_OUTPUT_BST_STRIP_ENDPOINTS_HPP__

#include <memory>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

#include "geometry/euclid_vector.hpp"
#include "geometry/line_segment.hpp"

#include "clas12/geometry.hpp"

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

using namespace clas12::geometry::central_tracker;

void bst_basic_parameters_xml(xml_document& doc, const BarrelSVT& bst, const string& coordsys="LAB", const string& units="mm")
{
double length_conversion = 1;
    if (units == "cm")
    {
        length_conversion = 0.1;
    }
    else if (units != "mm")
    {
        throw runtime_error(string("can not convert to units: ") + units);
    }

    if (coordsys != "LAB")
    {
        throw runtime_error(string("can not generate data in ") + coordsys + " coordinates");
    }

    // start building up the XML document
    xml_node geom_node = doc.child("geometry");
    if (!geom_node)
    {
        geom_node = doc.append_child("geometry");
    }

    xml_node bst_node = geom_node.child("barrel_svt");
    if (!bst_node)
    {
        bst_node = geom_node.append_child("barrel_svt");
    }

    xml_node bst_parms_node = bst_node.child("basic_parms");
     if (!bst_parms_node)
    {
        bst_parms_node = bst_node.append_child("basic_parms");
    }

        xml_node phi_node = bst_parms_node.append_child("phi");
        phi_node.append_attribute("coordinate_system") = coordsys.c_str();
        for (int i=0; i<bst.regions().size(); i++)
        {
            const barrel_svt::Region& reg = bst.region(i);
            stringstream ss;
            ss << reg.phi();
            phi_node.append_child(pugi::node_pcdata).set_value(ss.str().c_str());
        }
}



void bst_strip_endpoints_xml(xml_document& doc, const BarrelSVT& bst, const string& coordsys="LAB", const string& units="mm")
{
    double length_conversion = 1;
    if (units == "cm")
    {
        length_conversion = 0.1;
    }
    else if (units != "mm")
    {
        throw runtime_error(string("can not convert to units: ") + units);
    }

    if (coordsys != "LAB")
    {
        throw runtime_error(string("can not generate data in ") + coordsys + " coordinates");
    }

    // start building up the XML document
    xml_node geom_node = doc.child("geometry");
    if (!geom_node)
    {
        geom_node = doc.append_child("geometry");
    }

    xml_node bst_node = geom_node.child("barrel_svt");
    if (!bst_node)
    {
        bst_node = geom_node.append_child("barrel_svt");
    }

    xml_node strip_node = bst_node.child("strip_endpoints");
    if (!strip_node)
    {
        strip_node = bst_node.append_child("strip_endpoints");
        strip_node.append_attribute("length_units");
        strip_node.append_attribute("coordinate_system");
    }
    strip_node.attribute("length_units") = units.c_str();
    strip_node.attribute("coordinate_system") = coordsys.c_str();

    for (int reg=0; reg<bst.regions().size(); reg++)
    {
        const barrel_svt::Region& bst_region = bst.region(reg);

        //for (int sec=0; sec<bst_region.sectors().size(); sec++)
        for (int sec=0; sec<2; sec++)
        {
            const barrel_svt::Sector& bst_sector = bst_region.sector(sec);

            for (int lyr=0; lyr<bst_sector.layers().size(); lyr++)
            {
                const barrel_svt::Layer& bst_layer = bst_sector.layer(lyr);


                xml_node strip_endpoints_node = strip_node.append_child("strip_endpoints");
                strip_endpoints_node.append_attribute("sector") = sec;
                strip_endpoints_node.append_attribute("region") = reg;
                strip_endpoints_node.append_attribute("layer")  = lyr;

                xml_node left_node = strip_endpoints_node.append_child("first");
                vector<xml_node> endpoint_nodes;
                endpoint_nodes.push_back(left_node.append_child("x"));
                endpoint_nodes.push_back(left_node.append_child("y"));
                endpoint_nodes.push_back(left_node.append_child("z"));

                xml_node right_node = strip_endpoints_node.append_child("second");
                endpoint_nodes.push_back(right_node.append_child("x"));
                endpoint_nodes.push_back(right_node.append_child("y"));
                endpoint_nodes.push_back(right_node.append_child("z"));

                vector<stringstream> endpoints(6);
                for (line_segment<double,3> st : bst_layer.siliconStrips())
                {
                    euclid_vector<double,3> endpt = st.end_point();
                    endpoints[0] << " " << st.begin_point().x() * length_conversion;
                    endpoints[1] << " " << st.begin_point().y() * length_conversion;
                    endpoints[2] << " " << st.begin_point().z() * length_conversion;
                    endpoints[3] << " " << endpt.x() * length_conversion;
                    endpoints[4] << " " << endpt.y() * length_conversion;
                    endpoints[5] << " " << endpt.z() * length_conversion;
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

} // namespace clas12::geometry::output
} // namespace clas12::geometry
} // namespace clas12

#endif // __CLAS12_GEOMETRY_OUTPUT_DC_WIRE_ENDPOINTS_HPP__
