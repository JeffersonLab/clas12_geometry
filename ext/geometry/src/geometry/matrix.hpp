#ifndef __GEOMETRY_MATRIX_HPP__
#define __GEOMETRY_MATRIX_HPP__

#include <stdexcept>

#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/lu.hpp>
#include <boost/numeric/ublas/io.hpp>

/**
 * bounded_vector and bounded_matrix are the closest
 * thing boost::numeric::ublas has to small fixed-
 * sized vectors and matrices. The sizes are template
 * parameters but the size values are stored in the
 * object and can change (to a smaller value). A
 * proper "fixed" implementation is desirable and a
 * possibility for the future.
 *
 * An alternative would be blitz++ or tvmet. Both which
 * are excellent tiny-vector libraries.
 **/

#define fixed_matrix bounded_matrix

#include "geometry/rotation.hpp"
//#include "geometry/lorentz_transform.hpp"

namespace geometry
{

namespace blas = boost::numeric::ublas;

/// copied from boost ublas example in docs
/// Matrix inversion routine.
/// Uses lu_factorize and lu_substitute in uBLAS to invert a matrix
template <typename T>
blas::matrix<T> inverse(const blas::matrix<T>& input)
{
    typedef blas::permutation_matrix<std::size_t> pmatrix;

    blas::matrix<T> inverse;

    // create a working copy of the input
    blas::matrix<T> A(input);

    // create a permutation matrix for the LU-factorization
    pmatrix pm(A.size1());

    // perform LU-factorization
    int res = blas::lu_factorize(A,pm);
    if( res != 0 )
    {
        throw std::logic_error("Couldn't invert matrix.");
    }

    // create identity matrix of "inverse"
    inverse.assign(blas::identity_matrix<T>(A.size1()));

    // backsubstitute to get the inverse
    blas::lu_substitute(A, pm, inverse);

    return inverse;
}

} /* namespace geometry */


#endif /** __GEOMETRY_MATRIX_HPP__ **/
