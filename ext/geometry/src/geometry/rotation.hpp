#ifndef __GEOMETRY_EUCLID_TRANSFORM_HPP__
#define __GEOMETRY_EUCLID_TRANSFORM_HPP__

#include <cassert>
#include <cmath>
#include <limits>
#include <stdexcept>

#include "boost/assert.hpp"

#include "geometry/vector.hpp"
#include "geometry/matrix.hpp"

#include "geometry/detail/flags.hpp"

namespace geometry {

template <typename T, size_t dim>
class rotation
{
    BOOST_STATIC_ASSERT( dim > 1 && dim < 4 );
  private:
  protected:
  public:
    rotation() {}
    virtual ~rotation() {};
}; /** class rotation<T,dim> **/

/**
 * a 2D rotation from the x-axis, toward the y-axis
 * in radians
 **/
template <typename T>
class rotation <T,2>
    : boost::addable<           rotation<T,2>       /// r.angle + r.angle
    , boost::addable<           rotation<T,2>, T    /// r.angle + T, T + r.angle
    , boost::subtractable<      rotation<T,2>       /// r.angle - r.angle
    , boost::subtractable<      rotation<T,2>, T    /// r.angle - T
    , boost::subtractable2_left<rotation<T,2>, T    /// T - r.angle
    , boost::multipliable<      rotation<T,2>       /// r.angle * r.angle
    , boost::multipliable<      rotation<T,2>, T    /// r.angle * T, T * r.angle
    , boost::dividable<         rotation<T,2>       /// r.angle / r.angle
    , boost::dividable<         rotation<T,2>, T    /// r.angle / T
    , boost::dividable2_left<   rotation<T,2>, T    /// T / r.angle
    > > > > > > > > > >
{
    BOOST_STATIC_ASSERT( !std::numeric_limits<T>::is_integer );
  private:
    blas::fixed_matrix<T,2,2> data;
    T data_angle;
  protected:
  public:
    /**
     * "get" methods
     **/
    inline
    const T& angle() const { return data_angle; }
    inline
    const blas::fixed_matrix<T,2,2>& matrix() const { return data; }

    /**
     * "set" methods
     **/
    template <typename U>
    inline
    rotation& angle(const U& a)
    {
        data_angle = static_cast<T>(a);
        data(0,0) = std::cos(data_angle);
        data(1,0) = std::sin(data_angle);
        data(0,1) = -1 * data(1,0);
        data(1,1) = data(0,0);
        return *this;
    }

    /**
     * negative operation
     **/
    inline
    rotation& operator-()
    {
        return this->angle(-this->angle());
    }

    /**
     * scalar operations
     **/
    inline
    rotation& operator+=(const T& a)
    {
        return this->angle(this->angle() + a);
    }
    inline
    rotation& operator-=(const T& a)
    {
        return this->angle(this->angle() - a);
    }
    inline
    rotation& operator*=(const T& a)
    {
        return this->angle(this->angle() * a);
    }
    inline
    rotation& operator/=(const T& a)
    {
        return this->angle(this->angle() / a);
    }

    /**
     * binary operations
     **/
    inline
    rotation& operator+=(const rotation<T,2>& r)
    {
        return this->angle(this->angle() + r.angle());
    }
    inline
    rotation& operator-=(const rotation<T,2>& r)
    {
        return this->angle(this->angle() - r.angle());
    }
    inline
    rotation& operator*=(const rotation<T,2>& r)
    {
        return this->angle(this->angle() * r.angle());
    }
    inline
    rotation& operator/=(const rotation<T,2>& r)
    {
        return this->angle(this->angle() / r.angle());
    }

    /**
     * constructors
     **/
    /**
     * default constructor
     **/
    rotation() {}
    /**
     * constructor from angle
     **/
    template <typename U>
    rotation(const U& a)
    {
        this->angle(a);
    }

    /**
     * assignment operator()
     **/
    template <typename U>
    rotation& operator()(const U& a)
    {
        return this->angle(a);
    }

    /**
     * destructor
     **/
    virtual ~rotation() {};
}; /** class rotation<T,2> **/

/**
 * a 3D arbitrary rotation using euler angles in z-x-z orientation.
 **/
template <typename T>
class rotation <T,3>
{
    BOOST_STATIC_ASSERT( !std::numeric_limits<T>::is_integer );
  private:
    blas::fixed_matrix<T,3,3> data;
    blas::fixed_vector<T,3> data_euler_angles;
    template <typename U>
    inline
    void calculate_matrix_euler(
        const U& ca, const U& cb, const U& cg,
        const U& sa, const U& sb, const U& sg,
        unsigned int sequence = flags::zxz )
    {
        /// must have only one of the possible Euler angle sequences set
        assert((sequence & flags::zxz) != (sequence & flags::zyz));

        blas::fixed_matrix<T,3,3> R1;
        blas::fixed_matrix<T,3,3> R2;
        blas::fixed_matrix<T,3,3> R3;

        R1(0,0) = ca; R1(0,1) = -sa; R1(0,2) = 0;
        R1(1,0) = sa; R1(1,1) =  ca; R1(1,2) = 0;
        R1(2,0) = 0;  R1(2,1) =  0;  R1(2,2) = 1;

        if (sequence & flags::zxz)
        {
            R2(0,0) = 1; R2(0,1) = 0;  R2(0,2) =  0;
            R2(1,0) = 0; R2(1,1) = cb; R2(1,2) = -sb;
            R2(2,0) = 0; R2(2,1) = sb; R2(2,2) =  cb;
        }
        else // if (sequence & flags::zyz)
        {
            R2(0,0) =  cb; R2(0,1) = 0; R2(0,2) = sb;
            R2(1,0) =  0;  R2(1,1) = 1; R2(1,2) = 0;
            R2(2,0) = -sb; R2(2,1) = 0; R2(2,2) = cb;
        }

        R3(0,0) = cg; R3(0,1) = -sg; R3(0,2) = 0;
        R3(1,0) = sg; R3(1,1) =  cg; R3(1,2) = 0;
        R3(2,0) = 0;  R3(2,1) =  0;  R3(2,2) = 1;

        data = blas::prod(R3, blas::prod<blas::fixed_matrix<float,3,3> >(R2, R1));

        //data(0,0) = ca * cg - sa * cb * sg;
        //data(0,1) = -ca * sg - sa * cb * cg;
        //data(0,2) = sb * sa;
        //data(1,0) = sa * cg + ca * cb * sg;
        //data(1,1) = -sa * sg + ca * cb * cg;
        //data(1,2) = -sb * ca;
        //data(2,0) = sb * sg;
        //data(2,1) = sb * cg;
        //data(2,2) = cb;
    }
  protected:
  public:
    /**
     * "get" methods
     **/
    inline
    const blas::fixed_vector<T,3>& euler_angles() const { return data_euler_angles; }
    inline
    const blas::fixed_matrix<T,3,3>& matrix() const { return data; }

    /**
     * "set" methods
     **/
    /**
     * set by euler angles in z-x-z (or z-y-z) orientation
     **/
    template <typename U>
    inline
    rotation& euler_angles(const U& alpha, const U& beta, const U& gamma, unsigned int sequence = flags::zxz)
    {
        data_euler_angles[0] = alpha;
        data_euler_angles[1] = beta;
        data_euler_angles[2] = gamma;
        U ca = cos(alpha); U sa = sin(alpha);
        U cb = cos(beta);  U sb = sin(beta);
        U cg = cos(gamma); U sg = sin(gamma);
        calculate_matrix_euler(ca,cb,cg,sa,sb,sg,sequence);
        return *this;
    }
    /*
     * WARNING: ambiguous method using cosines!!!
    template <typename U>
    inline
    rotation& euler_angle_cosines(const U& ca, const U& cb, const U& cg)
    {
        data_euler_angles[0] = acos(ca);
        data_euler_angles[1] = acos(cb);
        data_euler_angles[2] = acos(cg);
        U sa = sin(data_euler_angles[0]);
        U sb = sin(data_euler_angles[1]);
        U sg = sin(data_euler_angles[2]);
        calculate_matrix_euler(ca,cb,cg,sa,sb,sg);
        return *this;
    }
    */

    /**
     * negative operation
     **/

    /**
     * scalar operations
     **/

    /**
     * binary operations
     **/

    /**
     * constructors
     **/
    /**
     * default constructor
     **/
    rotation() {}
    /**
     * constructor from angle
     **/
    template <typename U>
    rotation(const U& a, const U& b, const U& g)
    {
        this->euler_angles(a, b, g);
    }

    /**
     * destructor
     **/
    virtual ~rotation() {};
}; /** class rotation<T,3> **/

} /** namespace geometry **/

#endif /** __GEOMETRY_EUCLID_transform_HPP__ **/
