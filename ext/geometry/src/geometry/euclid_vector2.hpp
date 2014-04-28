#ifndef __GEOMETRY_EUCLID_VECTOR2_HPP__
#define __GEOMETRY_EUCLID_VECTOR2_HPP__

#include <iostream>
using std::clog;
using std::endl;

#include <cmath>
#include <limits>

#include <boost/operators.hpp>

#include "geometry/detail/debug.hpp"

#include "geometry/constants.hpp"
#include "geometry/euclid_vector.hpp"

namespace geometry
{

using std::numeric_limits;

using std::copysign;
using std::fabs;
using std::sqrt;
using std::pow;
using std::sin;
using std::cos;
using std::tan;
using std::asin;
using std::acos;
using std::atan2;

template <typename T>
class euclid_vector_base<T,2,coord::rec>
{
    typedef euclid_vector_base<T,2,coord::rec> thisclass_t;
    typedef euclid_vector<T,2,coord::rec> subclass_t;

  private:
    T _x;
    T _y;

  protected:
    euclid_vector_base()
    { DEBUG_CLOG("euclid_vector_base<T,2,rec> ctor\n") };

    euclid_vector_base(const T& x, const T& y)
    : _x(x), _y(y)
    { DEBUG_CLOG("euclid_vector_base<T,2,rec> ctor(x,y)\n") }

    // copy constructor
    euclid_vector_base(const thisclass_t& v)
    : _x(v._x), _y(v._y)
    { DEBUG_CLOG("euclid_vector_base<T,2,rec> copy ctor\n") }

    subclass_t& operator=(const thisclass_t& v)
    {
        DEBUG_CLOG("euclid_vector_base<T,2,rec> operator=\n")
        // This gracefully handles self assignment
        _x = v._x;
        _y = v._y;
        return *dynamic_cast<subclass_t*>(this);
    }

    // type-converting copy constructor
    template <typename U, coord::sys_t C>
    euclid_vector_base(const euclid_vector_base<U,2,C>& v)
    : _x(v.x()), _y(v.y())
    { DEBUG_CLOG("euclid_vector_base<T,2,rec> copy ctor<U,2,C>\n") }


    template <typename U, coord::sys_t C>
    subclass_t& operator=(const euclid_vector_base<U,2,C>& v)
    {
        DEBUG_CLOG("euclid_vector_base<T,2,rec> operator=<U,2,C>\n")
        _x = v.x();
        _y = v.y();
        return *dynamic_cast<subclass_t*>(this);
    }

    virtual
   ~euclid_vector_base() {};

  public:

    // accessor methods
    const T& x() const { return _x; }
    const T& y() const { return _y; }

    T r2()  const { return _x*_x + _y*_y; }
    T r()   const { return sqrt(r2()); }
    T phi() const
    {
        // phi in [-pi,pi]
        return atan2(_y,_x);
    }

    // set methods
    subclass_t& x(const T& x)
    {
        _x = x;
        return *dynamic_cast<subclass_t*>(this);
    }
    subclass_t& y(const T& y)
    {
        _y = y;
        return *dynamic_cast<subclass_t*>(this);
    }
    subclass_t& r(const T& r)
    {
        euclid_vector<T,2,coord::pol> v{r,this->phi()};
        _x = v.x();
        _y = v.y();
        return *dynamic_cast<subclass_t*>(this);
    }
    subclass_t& phi(const T& phi)
    {
        euclid_vector<T,2,coord::pol> v{this->r(),phi};
        _x = v.x();
        _y = v.y();
        return *dynamic_cast<subclass_t*>(this);
    }
};

template <typename T>
class euclid_vector_base<T,2,coord::pol>
{
    typedef euclid_vector_base<T,2,coord::pol> thisclass_t;
    typedef euclid_vector<T,2,coord::pol> subclass_t;

  private:
    T _r;
    T _phi;

    void validate()
    {
        DEBUG_CLOG("    euclid_vector_base<2,pol>::validate()")
        DEBUG_CLOG(" (" << _r << "," << _phi << ")")

        static const T tol = numeric_limits<T>::epsilon() * 10.;

        if (fabs(_r) < tol)
        {
            DEBUG_CLOG(" [r<tol] ")
            _r = 0.;
            _phi = 0.;
        }
        else
        {
            if (_r < 0)
            {
                _r = fabs(_r);
                _phi += cons::pi<T>();
            }

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
        DEBUG_CLOG(" --> (" << _r << "," << _phi << ")\n")
    }

  protected:
    euclid_vector_base()
    { DEBUG_CLOG("euclid_vector_base<T,2,pol> ctor\n") };

    euclid_vector_base(const T& r, const T& phi)
    : _r(r), _phi(phi)
    {
        DEBUG_CLOG("euclid_vector_base<T,2,pol> ctor(r,phi)\n")
        this->validate();
    }

    // copy constructor
    euclid_vector_base(const thisclass_t& v)
    : _r(v._r), _phi(v._phi)
    { DEBUG_CLOG("euclid_vector_base<T,2,pol> copy ctor\n") }

    subclass_t& operator=(const thisclass_t& v)
    {
        DEBUG_CLOG("euclid_vector_base<T,2,pol> operator=\n")
        // This gracefully handles self assignment
        _r = v._r;
        _phi = v._phi;
        return *dynamic_cast<subclass_t*>(this);
    }

    // type-converting copy constructor
    template <typename U, coord::sys_t C>
    euclid_vector_base(const euclid_vector_base<U,2,C>& v)
    : _r(v.r()), _phi(v.phi())
    {
        DEBUG_CLOG("euclid_vector_base<T,2,pol> copy ctor<U,2,C>\n")
    }

    template <typename U, coord::sys_t C>
    subclass_t& operator=(const euclid_vector_base<U,2,C>& v)
    {
        DEBUG_CLOG("euclid_vector_base<T,2,pol> operator=<U,2,C>\n")
        _r = v.r();
        _phi = v.phi();
        return *dynamic_cast<subclass_t*>(this);
    }

    virtual
   ~euclid_vector_base() {};

  public:

    // accessor methods
    T x() const { return _r*cos(_phi); }
    T y() const { return _r*sin(_phi); }

    T r2() const { return _r*_r; }
    const T& r() const { return _r; }
    const T& phi() const { return _phi; }

    // set methods
    subclass_t& r(const T& r)
    {
        _r = r;
        this->validate();
        return *dynamic_cast<subclass_t*>(this);
    }
    subclass_t& phi(const T& phi)
    {
        _phi = phi;
        this->validate();
        return *dynamic_cast<subclass_t*>(this);
    }
    subclass_t& x(const T& x)
    {
        euclid_vector<T,2,coord::rec> v{x,this->y()};
        _r = v.r();
        _phi = v.phi();
        return *dynamic_cast<subclass_t*>(this);
    }
    subclass_t& y(const T& y)
    {
        euclid_vector<T,2,coord::rec> v{this->x(),y};
        _r = v.r();
        _phi = v.phi();
        return *dynamic_cast<subclass_t*>(this);
    }
};

template <typename T, coord::sys_t coordsys>
class euclid_vector<T,2,coordsys>
: public euclid_vector_base<T,2,coordsys>
, boost::additive< euclid_vector<T,2,coordsys>
, boost::field_operators< euclid_vector<T,2,coordsys>, T
> >
{
    // coordinate type must be one of: rec, pol
    BOOST_STATIC_ASSERT( coordsys & (coord::rec|coord::pol) );

    typedef euclid_vector_base<T,2,coordsys> baseclass_t;
    typedef euclid_vector<T,2,coordsys>      thisclass_t;

  public:
    euclid_vector() { DEBUG_CLOG("euclid_vector<2> ctor\n") }

    euclid_vector(const T& x0, const T& x1)
    : baseclass_t(x0,x1)
    {
        DEBUG_CLOG("euclid_vector<2> ctor(x0,x1)\n")
    }
    template <class Vec>
    euclid_vector(const Vec& v)
    {
        DEBUG_CLOG("euclid_vector<2> copy ctor<Vec>\n")
        this->x(v.x()).y(v.y());
    }
    template <class Vec>
    euclid_vector& operator=(const Vec& v)
    {
        DEBUG_CLOG("euclid_vector<2> operator=<Vec>\n")
        return this->x(v.x()).y(v.y());
    }

    // copy constructor
    euclid_vector(const thisclass_t& v)
    : baseclass_t(v)
    {
        DEBUG_CLOG("euclid_vector<2> copy ctor\n")
    }
    euclid_vector& operator=(const thisclass_t& v)
    {
        DEBUG_CLOG("euclid_vector<2> operator=\n")
        // This gracefully handles self assignment
        baseclass_t::operator=(v);
        return *this;
    }

    // type-converting copy constructor
    template <typename U, coord::sys_t C>
    euclid_vector(const euclid_vector<U,2,C>& v)
    : baseclass_t(v)
    {
        DEBUG_CLOG("euclid_vector<T,2> copy ctor<U,2,C>\n")
    }
    template <typename U, coord::sys_t C>
    euclid_vector& operator=(const euclid_vector<U,2,C>& v)
    {
        DEBUG_CLOG("euclid_vector<T,2> operator=<U,2,C>\n")
        baseclass_t::operator=(v);
        return *this;
    }

    T cosphi() const { return cos(this->phi()); }

    thisclass_t operator-() const
    {
        thisclass_t vec(*this);
        return vec *= -1.;
    }

    // scalar operations
    thisclass_t& operator+=(const T& a)
    {
        return this->r(this->r() + a);
    }
    thisclass_t& operator-=(const T& a)
    {
        return this->r(this->r() - a);
    }
    thisclass_t& operator*=(const T& a)
    {
        return this->r(this->r() * a);
    }
    thisclass_t& operator/=(const T& a)
    {
        return this->r(this->r() / a);
    }

    // binary operations
    template <typename U, coord::sys_t C>
    thisclass_t& operator+=(const euclid_vector<U,2,C>& v)
    {
        return this->x(this->x()+v.x()).y(this->y()+v.y());
    }
    template <typename U, coord::sys_t C>
    thisclass_t& operator-=(const euclid_vector<U,2,C>& v)
    {
        return this->x(this->x()-v.x()).y(this->y()-v.y());
    }

    template <typename U, coord::sys_t C>
    thisclass_t operator+(const euclid_vector<U,2,C>& v) const
    {
        thisclass_t vec(*this);
        return vec += v;
    }
    template <typename U, coord::sys_t C>
    thisclass_t operator-(const euclid_vector<U,2,C>& v) const
    {
        thisclass_t vec(*this);
        return vec -= v;
    }

    /** \fn dot_prod(const euclid_vector<U,2,C>& v)
     * v1.dot_prod(v2)  ==  v1 . v2
     **/
    template <typename U, coord::sys_t C>
    T dot_prod(const euclid_vector<U,2,C>& v) const
    {
        return (this->x() * v.x() +  this->y() * v.y());
    }
    template <class Vec>
    T dot_prod(const Vec& v) const
    {
        long double xx = this->x() * v.x();
        long double yy = this->y() * v.y();
        return (xx + yy);
    }

    template <typename U, coord::sys_t C>
    T operator*(const euclid_vector<U,2,C>& v) const
    {
        return this->dot_prod(v);
    }

    template <typename U, coord::sys_t C>
    thisclass_t projection(const euclid_vector<U,2,C>& v) const
    {
        return (this->dot_prod(v) / (v*v)) * v;
    }
    template <class Vec>
    thisclass_t projection(const Vec& v) const
    {
        return this->projection(thisclass_t{T(v.x()),T(v.y())});
    }

    template <typename U, coord::sys_t C>
    T angle(const euclid_vector<U,2,C>& v) const
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

        DEBUG_CLOG("    euclid_vector<2>::angle(v)")
        DEBUG_CLOG(" " << v.phi() << " - " << this->phi())
        DEBUG_CLOG(" = " << dphi << endl)

        return dphi;
    }
    template <class Vec>
    T angle(const Vec& v) const
    {
        return this->angle(thisclass_t{T(v.x()),T(v.y())});
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
        if (dphi < tol)
        {
            return true;
        }
        return false;
    }
};

} /** namespace geometry **/

#endif /** __GEOMETRY_EUCLID_VECTOR2_HPP__ **/
