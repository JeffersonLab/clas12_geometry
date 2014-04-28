#ifndef __GEOMETRY_COORD_SYS_HPP__
#define __GEOMETRY_COORD_SYS_HPP__

namespace geometry {

namespace coord
{
    enum sys_t
    {
        rec = 1 << 0,
        pol = 1 << 1,
        cyl = 1 << 2, // cyl is not yet implemented
        sph = 1 << 3,

        // aliases for coordinate systems
        rectilinear = rec,
        rect        = rec,
        polar       = pol,
        cylindrical = cyl,
        spherical   = sph
    };
}

} /** namespace geometry **/

#endif /** __GEOMETRY_COORD_SYS_HPP__ **/

