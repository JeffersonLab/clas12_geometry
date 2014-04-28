#ifndef __GEOMETRY_LORENTZ_VECTOR_HPP__
#define __GEOMETRY_LORENTZ_VECTOR_HPP__

#include <cassert>
#include <cmath>
#include <stdexcept>

#include <boost/operators.hpp>

#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/symmetric.hpp>
#include <boost/numeric/ublas/matrix_proxy.hpp>

#include <boost/numeric/ublas/io.hpp>

#include "geometry/vector.hpp"
#include "geometry/detail/flags.hpp"

namespace geometry {

namespace blas = boost::numeric::ublas;

using std::fabs;
using std::pow;
using std::sqrt;

/**
 * lorentz_vector
 *
 * This is a 4 dimensional vector of the form (t,x,y,z)
 *
 **/
template <typename T=double>
class lorentz_vector
: boost::additive< lorentz_vector<T> >
{
  private:
    T _t;
    euclid_vector<T,3> _v;
  protected:
  public:
    /* *
     * "get" methods
     * */
    const
    T& temporal_component() const
    {
        return _t;
    }
    const
    euclid_vector<T,3>& spatial_vector() const
    {
        return _v;
    }
    blas::fixed_vector<T,4> vector() const
    {
        blas::fixed_vector<T,4> v;
        v[0] = _t;
        v[1] = _v.x();
        v[2] = _v.y();
        v[3] = _v.z();
        return v;
    }

    /* *
     * short hand get methods
     * */
    const T& t() const { return _t; }
    const T& x() const { return _v.x(); }
    const T& y() const { return _v.y(); }
    const T& z() const { return _v.z(); }

    const euclid_vector<T,3>& v() const { return _v; }

    const T& e() const { return _t; }

    T rho()      const { return _v.rho(); }
    T phi()      const { return _v.phi(); }
    T theta()    const { return _v.theta(); }
    T cosphi()   const { return _v.cosphi(); }
    T costheta() const { return _v.costheta(); }

    T m()  const { return this->invariant(); }
    T m2() const { return this->invariant2(); }

    T l()  const { return _v.length(); }
    T l2() const { return _v.length2(); }
    T p()  const { return _v.length(); }
    T p2() const { return _v.length2(); }

    T b() const { return this->beta(); }


    /* *
     * calculated get methods
     * */
    /**
     * invariant2 = t^2 - (x^2 + y^2 + z^2)
     **/
    T invariant2() const
    {
        T inv_sq = _t*_t - _v.length2();
        return inv_sq;
    }
    /**
     * invariant = +/- sqrt[ t^2 - (x^2 + y^2 + z^2) ]
     **/
    T invariant() const
    {
        long double inv2 = static_cast<long double>(this->invariant2());

        if (inv2 < 0.)
        {
            return (-1. * sqrt(fabs(inv2)));
        }
        return sqrt(inv2);
    }
    /**
     * beta = sqrt(x^2 + y^2 + z^2) / t
     **/
    T beta() const
    {
        return _v.length() / _t;
    }
    /**
     * beta_vector = \vec{v} / t
     **/
    euclid_vector<T,3> beta_vector() const
    {

        return _v / _t;
    }
    /**
     * gamma = 1 / sqrt(1 - beta^2)
     **/
    T gamma() const
    {
        return 1. / sqrt(1. - pow(this->beta(),2));
    }


    blas::symmetric_matrix<T> lorentz_boost() const
    {
        /// 4x4 lorentz boost matrix
        blas::symmetric_matrix<T> boost_matrix(4,4);

        T gamma = this->gamma();
        T beta2 = pow(this->beta(),2);
        euclid_vector<T,3> beta = this->beta_vector();
        blas::fixed_vector<T,3> beta_vector{beta.x(),beta.y(),beta.z()};
        T a = ((gamma - 1.) / beta2);

        boost_matrix(0,0) = gamma;
        boost_matrix(0,1) = -1. * beta.x() * gamma;
        boost_matrix(0,2) = -1. * beta.y() * gamma;
        boost_matrix(0,3) = -1. * beta.z() * gamma;

        /// 3x3 spatial part of boost matrix
        blas::matrix_range<blas::symmetric_matrix<T> > boost_submat
            (boost_matrix, blas::range(1,4), blas::range(1,4));

        /// sub-matrix = I + a * \vec{beta}^T \vec{beta}
        boost_submat = blas::identity_matrix<T>(3)
            + a * blas::outer_prod(beta_vector, beta_vector);

        return boost_matrix;
    }

    lorentz_vector lorentz_boost(const blas::symmetric_matrix<T>& boost_matrix) const
    {
        blas::fixed_vector<T,4> v = blas::prod(boost_matrix, this->vector());
        return lorentz_vector<T>(v);
    }

    lorentz_vector operator*(const blas::symmetric_matrix<T>& boost_matrix) const
    {
        return this->lorentz_boost(boost_matrix);
    }

    lorentz_vector lorentz_boost(const lorentz_vector& rest_frame_momentum) const
    {
        return this->lorentz_boost(rest_frame_momentum.lorentz_boost());
    }

    /* *
     * "set" methods
     * */
    template <typename U>
    lorentz_vector& t(const U& t)
    {
        _t = static_cast<T>(t);
        return *this;
    }
    template <typename U>
    lorentz_vector& v(const U& x, const U& y, const U& z)
    {
        _v.set(x,y,z);
        return *this;
    }

    lorentz_vector& v(const euclid_vector<T,3>& v)
    {
        _v = v;
        return *this;
    }
    template <typename U>
    lorentz_vector& x(const U& x)
    {
        _v.x(static_cast<T>(x));
        return *this;
    }
    template <typename U>
    lorentz_vector& y(const U& y)
    {
        _v.y(static_cast<T>(y));
        return *this;
    }
    template <typename U>
    lorentz_vector& z(const U& z)
    {
        _v.z(static_cast<T>(z));
        return *this;
    }

    /* *
     * modifying methods
     * */
    /**
     * invariant2(inv2, keep_const)
     * changes the invariant of the lorentz vector while keeping
     * either the spatial or temporal part constant.
     *
     * The direction of the spatial vector is not changed.
     **/
    template <typename U>
    lorentz_vector& invariant2(const U& inv2, unsigned int keep_const = flags::spatial)
    {
        if (keep_const & flags::temporal)
        {
            euclid_vector<T,3> v = this->v();
            T l2 = std::pow(_t,2) - static_cast<T>(inv2);
            if (l2 < 0.)
            {
                v.length(-1. * std::sqrt(std::abs(l2)));
            }
            else
            {
                v.length(std::sqrt(l2));
            }
            this->v(v);
        }
        else /* keep_const & flags::spatial */
        {
            T t2 = static_cast<T>(inv2) + this->spatial_vector().length2();
            if (t2 < 0.)
            {
                this->t(-1. * std::sqrt(std::abs(t2)));
            }
            else
            {
                this->t(std::sqrt(t2));
            }
        }
        return *this;
    }
    /**
     * invariant(inv, keep_const)
     * changes the invariant of the lorentz vector while keeping
     * either the spatial or temporal part constant.
     *
     * The direction of the spatial vector is not changed.
     **/
    template <typename U>
    lorentz_vector& invariant(const U& inv, unsigned int keep_const = flags::spatial)
    {
        U inv2;
        if (inv < 0)
        {
            inv2 = -1. * std::pow(inv,2);
        }
        else
        {
            inv2 = std::pow(inv,2);
        }
        return this->invariant2(inv2, keep_const);
    }
    /**
     * rotation of the spatial vector only
     * using cosine-euler-angles in z-x-z orientation
     **/
    template <typename U, typename V, typename W>
    lorentz_vector& rotate(const U& a, const V& b, const W& g)
    {
        euclid_vector<T,3> v = this->v();
        this->v(v.rotate(a,b,g));
        return *this;
    }
    /**
     * rotation by rotation object
     **/
    template <typename U>
    lorentz_vector& rotate(const rotation<U,3>& r)
    {
        euclid_vector<T,3> v = this->v();
        this->v(v.rotate(r));
        return *this;
    }

    /* *
     * binary operations
     * */
    lorentz_vector& operator+=(const lorentz_vector<T>& v)
    {
        return this->
            t(_t + v.t()).
            v(_v + v.v());
    }
    lorentz_vector& operator-=(const lorentz_vector<T>& v)
    {
        return this->
            t(_t - v.t()).
            v(_v - v.v());
    }

    /**
     * serialization
     *
     * this single method is used for saving and loading
     * so the variables accessed need to be non-const type.
     **/
    friend class boost::serialization::access;
    template<class Archive>
    void serialize(Archive & ar, const unsigned int version)
    {
        ar & _t;
        ar & _v;
    }

    /* *
     * constructors
     * */
    lorentz_vector() {}
    lorentz_vector(const T& t, const T& x, const T& y, const T& z)
    : _t(t)
    , _v(x,y,z)
    {
    }
    template<typename U>
    lorentz_vector(blas::fixed_vector<U,4> v)
    : _t(v[0])
    , _v(v[1], v[2], v[3])
    {
    }

    /* *
     * destructor
     * */
    virtual ~lorentz_vector() {};

}; /* class lorentz_vector */

} /* namespace geometry */

#endif /* __GEOMETRY_LORENTZ_VECTOR_HPP__ */
