#ifndef __GEOMETRY_EUCLID_VECTOR_HPP__
#define __GEOMETRY_EUCLID_VECTOR_HPP__

#include <cassert>
#include <limits>

#include <boost/static_assert.hpp>

#include "geometry/detail/debug.hpp"

#include "geometry/coord_sys.hpp"

namespace geometry
{

template <typename T, size_t dim, coord::sys_t coordsys>
class euclid_vector_base
{
    // euclid vectors only make sense for 2 and 3 dimensions
    BOOST_STATIC_ASSERT( dim > 1 && dim < 4 );

    // the type storing the coordinates must be real (not integral)
    BOOST_STATIC_ASSERT( !std::numeric_limits<T>::is_integer );

  protected:
    euclid_vector_base() { DEBUG_CLOG("euclid_vector_base ctor.\n") };
};

template <typename T=double, size_t dim=3, coord::sys_t coordsys=coord::rec>
class euclid_vector
: public euclid_vector_base<T,dim,coordsys>
{
    euclid_vector()=delete;
};

} /** namespace geometry **/

#include "euclid_vector2.hpp"
#include "euclid_vector3.hpp"

#endif /** __GEOMETRY_EUCLID_VECTOR_HPP__ **/
