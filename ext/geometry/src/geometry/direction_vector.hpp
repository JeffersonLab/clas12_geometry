#ifndef __GEOMETRY_DIRECTION_VECTOR_HPP__
#define __GEOMETRY_DIRECTION_VECTOR_HPP__

#include <cassert>
#include <limits>

#include "geometry/detail/debug.hpp"

#include "geometry/euclid_vector.hpp"

namespace geometry
{

using std::numeric_limits;

template <typename T=double, size_t dim=3>
class direction_vector
{
    // direction vectors only make sense for 2 and 3 dimensions
    BOOST_STATIC_ASSERT( dim > 1 && dim < 4 );

    // the type storing the coordinates must be real (not integral)
    BOOST_STATIC_ASSERT( !std::numeric_limits<T>::is_integer );

  protected:
    direction_vector() = delete;
};

template <typename T, size_t dim, coord::sys_t C>
T operator*(
    const euclid_vector<T,dim,C>& v1,
    const direction_vector<T,dim>& v2)
{
    return v2 * v1;
}

template <typename T, size_t dim, coord::sys_t C>
euclid_vector<T,dim,C> operator+(
    const euclid_vector<T,dim,C>& v1,
    const direction_vector<T,dim>& v2)
{
    return v2 + v1;
}

template <typename T, size_t dim, coord::sys_t C>
euclid_vector<T,dim,C> operator-(
    const euclid_vector<T,dim,C>& v1,
    const direction_vector<T,dim>& v2)
{
    return -(v2 - v1);
}

template <typename T, size_t dim, coord::sys_t C>
euclid_vector<T,dim,C> operator^(
    const euclid_vector<T,dim,C>& v1,
    const direction_vector<T,dim>& v2)
{
    return -(v2 ^ v1);
}

template <typename T, size_t dim, coord::sys_t C>
euclid_vector<T,dim,coord::rec> projection(
    const euclid_vector<T,dim,C>& v1,
    const direction_vector<T,dim>& v2)
{
    return (v1.dot_prod(v2) / (v2*v2)) * v2;
}

} /* namespace geometry */

#include "direction_vector2.hpp"
#include "direction_vector3.hpp"

#endif /** __GEOMETRY_DIRECTION_VECTOR_HPP__ **/
