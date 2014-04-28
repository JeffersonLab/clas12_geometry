#ifndef __CLAS12_GEOMETRY_DETAIL_MULTI_ARRAY_HPP__
#define __CLAS12_GEOMETRY_DETAIL_MULTI_ARRAY_HPP__

#include <array>

namespace clas12
{
namespace geometry
{

/**
 * The following set of template typedefs define the "multi_array"
 * object which behaves just like std::array, but can be any
 * number of dimensions. For example, to create a 3x4 "matrix",
 * one could do this:
 *
 *      multi_array<float,3,4> A;
 *
 * This is equivalent to (but more type-safe):
 *
 *      float[3][4] A;
 *
 * The single-dimensional case: type multi_array<float,3>
 * is identical to std::array<float,3>
 **/
template <typename T, size_t A, size_t... B>
struct multi_array_t
{
    using type = std::array<typename multi_array_t<T,B...>::type, A>;
};

template <typename T, size_t A>
struct multi_array_t<T, A>
{
    using type = std::array<T,A>;
};

template <typename T, size_t... A>
using multi_array = typename multi_array_t<T, A...>::type;

} /* namespace clas12::geometry */
} /* namespace clas12 */

#endif /* __CLAS12_GEOMETRY_DETAIL_MULTI_ARRAY_HPP__ */
