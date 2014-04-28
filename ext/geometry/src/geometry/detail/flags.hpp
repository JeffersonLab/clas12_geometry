#ifndef __GEOMETRY_DETAIL_FLAGS_HPP__
#define __GEOMETRY_DETAIL_FLAGS_HPP__

namespace geometry
{

namespace flags {
    enum geometry_flags_t {
        temporal = 1 << 0,
        spatial  = 1 << 1,
        zxz      = 1 << 2,
        zyz      = 1 << 3
    };
} // namespace flags

}

#endif // __GEOMETRY_DETAIL_FLAGS_HPP__
