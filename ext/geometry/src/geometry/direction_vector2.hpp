#ifndef __GEOMETRY_DIRECTION_VECTOR2_HPP__
#define __GEOMETRY_DIRECTION_VECTOR2_HPP__

#include <cassert>
#include <cmath>

#include "geometry/detail/debug.hpp"

#include "geometry/constants.hpp"
#include "geometry/euclid_vector.hpp"
#include "geometry/direction_vector.hpp"

namespace geometry
{

using std::numeric_limits;

using std::fmod;
using std::abs;
using std::sqrt;
using std::pow;
using std::sin;
using std::cos;
using std::tan;
using std::asin;
using std::acos;
using std::atan2;

template <typename T, coord::sys_t C>
class euclid_vector<T,2,C>;

template <typename T>
class direction_vector<T,2>
{
    typedef direction_vector<T,2> thisclass_t;
  private:
    T _phi;

    void validate()
    {
        static const T tol = numeric_limits<T>::epsilon() * 10.;

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
    }

  public:
    direction_vector()
    { DEBUG_CLOG("direction_vector<2> ctor") }

    direction_vector(const T& phi)
    : _phi(phi)
    {
        DEBUG_CLOG("direction_vector<2> ctor(phi)")
        this->validate();
    }
    direction_vector(const T& x, const T& y)
    : _phi(atan2(y,x))
    {
        DEBUG_CLOG("direction_vector<2> ctor(phi)")
    }
    template <typename U, coord::sys_t C>
    direction_vector(const euclid_vector<U,2,C> v)
    : _phi(T(v.phi()))
    {
        DEBUG_CLOG("direction_vector<2> ctor(euclid_vector)")
    }
    template <typename U, coord::sys_t C>
    direction_vector& operator=(const euclid_vector<U,2,C>& v)
    {
        DEBUG_CLOG("direction_vector<2> operator=<euclid_vector>\n")
        return this->phi(T(v.phi()));
    }

    // copy constructor
    direction_vector(const thisclass_t& v)
    : _phi(v._phi)
    { DEBUG_CLOG("direction_vector<2> copy ctor") }

    thisclass_t& operator=(const thisclass_t& v)
    {
        DEBUG_CLOG("direction_vector<2> operator=")
        // This gracefully handles self assignment
        _phi = v._phi;
        return *this;
    }

    // type-converting copy constructor
    template <typename U>
    direction_vector(const direction_vector<U,2>& v)
    : _phi(v.phi())
    {
        DEBUG_CLOG("direction_vector<T,2> copy ctor<U>")
    }
    template <typename U>
    thisclass_t& operator=(const direction_vector<U,2>& v)
    {
        DEBUG_CLOG("direction_vector<T,2> operator=<U>")
        _phi = v.phi();
        return *this;
    }

   ~direction_vector() {};

    //constexpr
    T r() const { return 1.; }
    //constexpr
    T r2() const { return 1.; }

    const T& phi() const { return _phi; }
    T cosphi() const { return cos(_phi); }

    T x() const { return cos(_phi); }
    T y() const { return sin(_phi); }

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

    thisclass_t operator-() const
    {
        return thisclass_t{
            fmod(_phi + cons::pi<T>(), cons::two_pi<T>())
        };
    }

    template <typename U>
    euclid_vector<T,2,coord::pol> operator*(const U& a) const
    {
        return euclid_vector<T,2,coord::pol>{T(a),_phi};
    }
    template <typename U>
    euclid_vector<T,2,coord::pol> operator/(const U& a) const
    {
        return euclid_vector<T,2,coord::pol>{T(1.)/T(a),_phi};
    }

    euclid_vector<T,2,coord::rec> operator+(const thisclass_t& v) const
    {
        return euclid_vector<T,2,coord::rec>{
            T(this->x() + v.x()),
            T(this->y() + v.y())};
    }
    euclid_vector<T,2,coord::rec> operator-(const thisclass_t& v) const
    {
        return euclid_vector<T,2,coord::rec>{
            T(this->x() - v.x()),
            T(this->y() - v.y())};
    }

    template <typename U, coord::sys_t C>
    euclid_vector<T,2,coord::rec> operator+(const euclid_vector<U,2,C>& v) const
    {
        return euclid_vector<T,2,coord::rec>{
            T(this->x() + v.x()),
            T(this->y() + v.y())};
    }
    template <typename U, coord::sys_t C>
    euclid_vector<T,2,coord::rec> operator-(const euclid_vector<U,2,C>& v) const
    {
        return euclid_vector<T,2,coord::rec>{
            T(this->x() - v.x()),
            T(this->y() - v.y())};
    }

    /** \fn dot_prod(const euclid_vector<U,2,C>& v)
     * v1.dot_prod(v2)  ==  v1 . v2
     **/
    template <class Vec>
    T dot_prod(const Vec& v) const
    {
        long double xx = this->x() * v.x();
        long double yy = this->y() * v.y();
        return (xx + yy);
    }

    T operator*(const thisclass_t& v) const
    {
        return this->dot_prod(v);
    }
    template <typename U>
    T operator*(const direction_vector<U,2>& v) const
    {
        return this->dot_prod(v);
    }
    template <typename U, coord::sys_t C>
    T operator*(const euclid_vector<U,2,C>& v) const
    {
        return this->dot_prod(v);
    }

    template <class Vec>
    euclid_vector<T,2,coord::rec> projection(const Vec& v) const
    {
        return (this->dot_prod(v) / (v*v)) * v;
    }

    template <class Vec>
    T angle(const Vec& v) const
    {
        static const T tol = numeric_limits<T>::epsilon() * 10.;
        T dphi = fabs(v.phi() - this->phi());
        if (dphi > cons::pi<T>())
        {
            dphi = cons::two_pi<T>() - dphi;
        }
        if (dphi < tol)
        {
            dphi = 0.;
        }
        return dphi;
    }

    template <class Vec>
    bool is_parallel(
        const Vec& v,
        const T& tol = std::numeric_limits<T>::epsilon() * 10.
        ) const
    {
        T dphi = abs(_phi - v.phi());
        if (dphi > 1.)
        {
            dphi -= cons::two_pi<T>();
        }
        if (dphi < tol)
        {
            return true;
        }
        return false;
    }
};

template <typename T>
euclid_vector<T,2,coord::pol> operator*(
    const float& a,
    const direction_vector<T,2>& v )
{
    return v * T(a);
}
template <typename T>
euclid_vector<T,2,coord::pol> operator*(
    const double& a,
    const direction_vector<T,2>& v )
{
    return v * T(a);
}
template <typename T>
euclid_vector<T,2,coord::pol> operator*(
    const long double& a,
    const direction_vector<T,2>& v )
{
    return v * T(a);
}

} /* namespace geometry */

#endif /** __GEOMETRY_DIRECTION_VECTOR2_HPP__ **/

