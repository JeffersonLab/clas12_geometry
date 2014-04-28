#ifndef CLAS12_GEOMETRY_OUTPUT_DETAIL_LENGTH_CONVERSION_HPP
#define CLAS12_GEOMETRY_OUTPUT_DETAIL_LENGTH_CONVERSION_HPP

#include <string>
#include <stdexcept>

namespace clas12
{
namespace geometry
{
namespace output
{

using std::runtime_error;
using std::string;

double length_conversion(const string& units)
{
    double l = 1;
    if (units == "mm")
    {
        l = 10;
    }
    else if (units == "m")
    {
        l = 0.01;
    }
    else if (units != "cm")
    {
        throw runtime_error(string("can not convert to units: ") + units);
    }
    return l;
}

} // namespace clas12::geometry::output
} // namespace clas12::geometry
} // namespace clas12

#endif // CLAS12_GEOMETRY_OUTPUT_DETAIL_LENGTH_CONVERSION_HPP
