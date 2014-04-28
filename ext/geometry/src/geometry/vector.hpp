#ifndef __GEOMETRY_VECTOR_HPP__
#define __GEOMETRY_VECTOR_HPP__

namespace geometry
{

/**
 * Euclidean vectors:
 * use euclid_vector<T,dim>
 * where T is any numeric type
 * for vectors with an identity metric
 *
 * three-vector:
 *   euclid_vector<T, 3>
 * general 3D vector which obeys usual BLAS rules
 *
 * dircos-vector:
 *   dircos_vector<T>
 * direction cosines vector. Basically a unit vector
 * in [cos(alpha), cos(beta), cos(gamma)] form.
 **/

/**
 * Lorentz vectors:
 * use this for standard lorentz vector
 * with metric (1, -1, -1, -1)
 *
 * lorentz (four) vector:
 *   lorentz_vector<T>
 * where T is any floating point numeric type.
 * basic lorentz four-vector with available methods
 * that obey the metric above:
 *   T lorentz_vector<T>::invariant()
 *   T lorentz_vector<T>::invariant_sq()
 *   euclid_vector<T,3> lorentz_vector<T>::spatial_vector()
 **/

} /** namespace geometry **/

#include "geometry/euclid_vector.hpp"
#include "geometry/direction_vector.hpp"
//#include "geometry/lorentz_vector.hpp"

#endif /** __GEOMETRY_VECTOR_HPP__ **/
