#ifndef __GEOMETRY_DIRECTION_VECTOR3_HPP__
#define __GEOMETRY_DIRECTION_VECTOR3_HPP__

#include <iostream>
using std::clog;
using std::endl;

#include <cassert>
#include <cmath>
#include <limits>

#include "geometry/detail/debug.hpp"

#include "geometry/constants.hpp"
#include "geometry/euclid_vector.hpp"
#include "geometry/direction_vector.hpp"

namespace geometry
{

using std::numeric_limits;

using std::abs;
using std::sqrt;
using std::pow;
using std::sin;
using std::cos;
using std::tan;
using std::asin;
using std::acos;
using std::atan2;

template <typename T>
class direction_vector<T,3>
{
    typedef direction_vector<T,3> thisclass_t;

  private:
    T _phi;
    T _theta;

    void validate()
    {
        static const T tol = numeric_limits<T>::epsilon() * 100.;

        // phi in [-pi,pi]
        _phi  = fmod(_phi,cons::two_pi<T>());
        if (fabs(_phi) >= cons::pi<T>())
        {
            _phi -= copysign(cons::two_pi<T>(),_phi);
        }
        if (fabs(_phi) < tol)
        {
            _phi = 0.;
        }

        // theta in [0,pi)
        _theta = fabs(_theta);
        if (_theta > cons::pi<T>())
        {
            _theta = fmod(_theta,cons::two_pi<T>());
            if (_theta >= cons::pi<T>())
            {
                _theta = cons::two_pi<T>() - _theta;
            }
        }
        if (_theta < tol)
        {
            _phi = 0.;
            _theta = 0.;
        }
    }

  public:
    direction_vector()
    {
        DEBUG_CLOG("direction_vector<3> ctor")
    }

    direction_vector(const T& phi, const T& theta)
    : _phi(phi), _theta(theta)
    {
        DEBUG_CLOG("direction_vector<3> ctor(phi,theta)")
        this->validate();
    }
    direction_vector(const T& x, const T& y, const T& z)
    {
        DEBUG_CLOG("direction_vector<3> ctor(x,y,z)")
        euclid_vector<T,3,coord::rec> v{x,y,z};
        _phi = v.phi();
        _theta = v.theta();
    }
    template <typename U, coord::sys_t C>
    direction_vector(const euclid_vector<U,3,C> v)
    : _phi(T(v.phi())), _theta(T(v.theta()))
    {
        DEBUG_CLOG("direction_vector<3> ctor(euclid_vector)")
    }
    template <typename U, coord::sys_t C>
    direction_vector& operator=(const euclid_vector<U,3,C>& v)
    {
        DEBUG_CLOG("direction_vector<3> operator=<euclid_vector>\n")
        return this->phi(T(v.phi())).theta(T(v.theta()));
    }

    // copy constructor
    direction_vector(const thisclass_t& v)
    : _phi(v._phi), _theta(v._theta)
    {
        DEBUG_CLOG("direction_vector<3> copy ctor")
    }
    thisclass_t& operator=(const thisclass_t& v)
    {
        DEBUG_CLOG("direction_vector<3> operator=")
        // This gracefully handles self assignment
        _phi = v._phi;
        _theta = v._theta;
        return *this;
    }

    // type-converting copy constructor
    template <typename U>
    direction_vector(const direction_vector<U,3>& v)
    : _phi(v.phi()), _theta(v.theta())
    {
        DEBUG_CLOG("direction_vector<T,3> copy ctor<U>")
    }
    template <typename U>
    thisclass_t& operator=(const direction_vector<U,3>& v)
    {
        DEBUG_CLOG("direction_vector<T,3> operator=<U>")
        _phi = v.phi();
        _theta = v.theta();
        return *this;
    }

   ~direction_vector() {};

    //constexpr
    T r() const { return 1.; }
    //constexpr
    T r2() const { return 1.; }

    const T& phi() const { return _phi; }
    const T& theta() const { return _theta; }

    T rho() const { return sin(_theta); }
    T rho2() const { return pow(this->rho(),2); }

    T x() const { return this->rho()*cos(_phi); }
    T y() const { return this->rho()*sin(_phi); }
    T z() const { return cos(_theta); }

    T costheta() const { return cos(_theta); }

    thisclass_t& phi(const T& phi)
    {
        _phi = phi;
        this->validate();
        return *this;
    }
    thisclass_t& cosphi(const T& cosphi)
    {
        assert( abs(cosphi) <= 1. );
        _phi = acos(cosphi);
        return *this;
    }
    thisclass_t& theta(const T& theta)
    {
        _theta = theta;
        this->validate();
        return *this;
    }
    thisclass_t& costheta(const T& costheta)
    {
        assert( abs(costheta) <= 1. );
        _theta = acos(costheta);
        this->validate();
        return *this;
    }

    thisclass_t operator-() const
    {
        return thisclass_t{
            fmod(_phi + cons::pi<T>(), cons::two_pi<T>()),
            cons::pi<T>() - _theta };
    }

    template <typename U>
    euclid_vector<T,3,coord::sph> operator*(const U& a) const
    {
        return euclid_vector<T,3,coord::sph>{T(a),_phi,_theta};
    }
    template <typename U>
    euclid_vector<T,3,coord::sph> operator/(const U& a) const
    {
        return euclid_vector<T,3,coord::sph>{T(1.)/T(a),_phi,_theta};
    }

    euclid_vector<T,3,coord::rec> operator+(const thisclass_t& v) const
    {
        return euclid_vector<T,3,coord::rec>{
            T(this->x() + v.x()),
            T(this->y() + v.y()),
            T(this->z() + v.z())};
    }
    euclid_vector<T,3,coord::rec> operator-(const thisclass_t& v) const
    {
        return euclid_vector<T,3,coord::rec>{
            T(this->x() - v.x()),
            T(this->y() - v.y()),
            T(this->z() - v.z())};
    }

    template <typename U, coord::sys_t C>
    euclid_vector<T,3,coord::rec> operator+(const euclid_vector<U,3,C>& v) const
    {
        return euclid_vector<T,3,coord::rec>{
            T(this->x() + v.x()),
            T(this->y() + v.y()),
            T(this->z() + v.z())};
    }
    template <typename U, coord::sys_t C>
    euclid_vector<T,3,coord::rec> operator-(const euclid_vector<U,3,C>& v) const
    {
        return euclid_vector<T,3,coord::rec>{
            T(this->x() - v.x()),
            T(this->y() - v.y()),
            T(this->z() - v.z())};
    }

    /** \fn dot_prod(const euclid_vector<U,2,C>& v)
     * v1.dot_prod(v2)  ==  v1 . v2
     **/
    template <class Vec>
    T dot_prod(const Vec& v) const
    {
        euclid_vector<long double,3> this_vec(*this);
        euclid_vector<long double,3> that_vec(v);
        return this_vec.dot_prod(that_vec);
    }

    T operator*(const thisclass_t& v) const
    {
        return this->dot_prod(v);
    }
    template <typename U>
    T operator*(const direction_vector<U,3>& v) const
    {
        return this->dot_prod(v);
    }
    template <typename U, coord::sys_t C>
    T operator*(const euclid_vector<U,3,C>& v) const
    {
        return this->dot_prod(v);
    }

    euclid_vector<T,3,coord::rec> projection(const thisclass_t& v) const
    {
        return this->dot_prod(v) * v;
    }
    template <class Vec>
    euclid_vector<T,3,coord::rec> projection(const Vec& v) const
    {
        euclid_vector<long double,3> this_vec{this->x(),this->y(),this->z()};
        euclid_vector<long double,3> that_vec{v};
        return this_vec.projection(that_vec);
    }

    T angle(const thisclass_t& v) const
    {
        T a = this->dot_prod(v);
        return a<1 ? a>-1 ? acos(a) : cons::pi<T>() : 0;
    }
    template <class Vec>
    T angle(const Vec& v) const
    {
        euclid_vector<long double> v1{this->x(),this->y(),this->z()};
        euclid_vector<long double> v2{v.x(),v.y(),v.z()};
        return T(v1.angle(v2));
    }

    /**
     * cross product:
     * v1.cross_prod(v2)  ==  v1 x v2
     **/
    thisclass_t cross_prod(const thisclass_t& v) const
    {
        long double zy = this->z() * v.y();
        long double yz = this->y() * v.z();
        long double xz = this->x() * v.z();
        long double zx = this->z() * v.x();
        long double yx = this->y() * v.x();
        long double xy = this->x() * v.y();
        return thisclass_t{T(yz-zy), T(zx-xz), T(xy-yx)};
    }
    template <class Vec>
    euclid_vector<T,3,coord::rec> cross_prod(const Vec& v) const
    {
        euclid_vector<long double,3> this_vec{this->x(),this->y(),this->z()};
        euclid_vector<long double,3> that_vec{v};
        return this_vec.cross_prod(that_vec);
    }

    thisclass_t operator^(const thisclass_t& v) const
    {
        return this->cross_prod(v);
    }
    template <coord::sys_t C>
    euclid_vector<T,3,coord::rec> operator^(const euclid_vector<T,3,C>& v) const
    {
        return this->cross_prod(v);
    }

    template <class Vec>
    bool is_parallel(
        const Vec& v,
        const T& tol = std::numeric_limits<T>::epsilon() * 10.
        ) const
    {
        T dphi = abs(this->phi() - v.phi());
        if (dphi > 1.)
        {
            dphi -= cons::two_pi<T>();
        }
        if (dphi < tol &&
            abs(this->theta()   - v.theta()) < tol )
        {
            return true;
        }
        return false;
    }


}; // class direction_vector<T,3>

template <typename T>
euclid_vector<T,3,coord::sph> operator*(
    const float& a, const direction_vector<T,3>& v)
{
    return v * static_cast<T>(a);
}
template <typename T>
euclid_vector<T,3,coord::sph> operator*(
    const double& a, const direction_vector<T,3>& v)
{
    return v * static_cast<T>(a);
}
template <typename T>
euclid_vector<T,3,coord::sph> operator*(
    const long double& a, const direction_vector<T,3>& v)
{
    return v * static_cast<T>(a);
}

} // namespace geometry

#endif // __GEOMETRY_DIRECTION_VECTOR3_HPP__
