#ifndef CLAS12_GEOMETRY_OUTPUT_DC_CORE_PARAMS_HPP
#define CLAS12_GEOMETRY_OUTPUT_DC_CORE_PARAMS_HPP

#include <string>
#include <vector>

#include "clas12/geometry/drift_chamber.hpp"

#include "detail/length_conversion.hpp"

namespace clas12
{
namespace geometry
{
namespace output
{

using std::string;
using std::vector;

using pugi::xml_document;
using pugi::xml_node;

using clas12::geometry::DriftChamber;

void dc_core_params_xml(xml_document& doc, const DriftChamber& dc, const string& units="cm")
{
    double lconv = length_conversion(units);

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

    xml_node params_node = dc_node.append_child("core_params");

    for (int nreg : vector<int>{0, 1, 2})
    {
        xml_node reg_node = params_node.append_child("region");
        reg_node.append_attribute("index") = nreg;

        for (int nslyr : vector<int>{0, 1})
        {
            xml_node slyr_node = reg_node.append_child("superlayer");
            slyr_node.append_attribute("index") = nslyr;

            slyr_node.append_attribute("wpdist") = dc.sector(0).region(nreg).superlayer(nslyr).wpdist() * lconv;
        }
    }
}

} // namespace clas12::geometry::output
} // namespace clas12::geometry
} // namespace clas12

#endif // CLAS12_GEOMETRY_OUTPUT_DC_CORE_PARAMS_HPP
