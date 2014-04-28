#ifndef __GEOMETRY_HPP__
#define __GEOMETRY_HPP__

/**

\mainpage Geometry library

This project is a C++ library which contains basic geometric objects such as euclidean vectors, direction vectors, lines, planes and lorentz vectors along with some useful operations likes rotations, intersections, boosts, etc.

It is geared toward physical modeling and is optimized both for speed and small internal storage.

This is an example of using two euclid_vector objects to define a plane and taking their cross-product to get the normal to the plane. The length of the resulting vector is also the area of the parallelogram defined by the two original vectors.
\include examples/doc/euclid_vector_01.cpp

**/

#include "geometry/constants.hpp"
#include "geometry/vector.hpp"
#include "geometry/line.hpp"
#include "geometry/line_segment.hpp"
#include "geometry/plane.hpp"

#endif /** __GEOMETRY_HPP__ **/
