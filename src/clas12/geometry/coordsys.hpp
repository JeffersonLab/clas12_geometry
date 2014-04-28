#ifndef CLAS12_GEOMETRY_COORDSYS_HPP
#define CLAS12_GEOMETRY_COORDSYS_HPP

#include <string>

namespace clas12
{
namespace geometry
{

namespace COORDSYS
{

    using std::string;

    typedef enum {
        CLAS   = 0,
        SECTOR = 1,
        LAYER  = 2,

        // aliases
        CLAS12 = CLAS
    } coordsys_t;

    coordsys_t str2coord(const string& coord);
    string coord2str(coordsys_t coord);
}

using COORDSYS::coordsys_t;
using COORDSYS::str2coord;
using COORDSYS::coord2str;

} // namespace clas12::geometry
} // namespace clas12

#endif // CLAS12_GEOMETRY_COORDSYS_HPP
