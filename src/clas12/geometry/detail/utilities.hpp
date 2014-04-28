#ifndef __CLAS12_GEOMETRY_DETAIL_UTILITIES_HPP__
#define __CLAS12_GEOMETRY_DETAIL_UTILITIES_HPP__

#include <array>
#include <cstdlib>
#include <iostream>
#include <sstream>
#include <string>
#include <stdexcept>

namespace clas12
{
namespace geometry
{

using namespace std;

int find_element(const vector<string>& vec, const string& elem)
{
    return distance(vec.begin(), find(vec.begin(), vec.end(), elem));
}

template <typename T>
inline
T lexical_cast(const string& str)
{
    T ret;
    if (!(stringstream(str) >> boolalpha >> ret))
    {
        stringstream ss;
        ss << "Could not convert: '" << str << "' to a numeric type.";
        throw logic_error(ss.str());
    }
    return ret;
}

template <typename ContainerType>
void print_container(const ContainerType& v)
{
    for (auto i=v.begin(); i<v.end(); ++i)
    {
        clog << " " << *i;
    }
    clog << endl;
}

} /* namespace clas12::geometry */
} /* namespace clas12 */


#endif /* __CLAS12_GEOMETRY_DETAIL_UTILITIES_HPP__ */

