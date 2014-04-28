#ifndef __GEOMETRY_EUCLID_VECTOR3_HPP__
#define __GEOMETRY_EUCLID_VECTOR3_HPP__

#include <cmath>
#include <limits>

#include <boost/operators.hpp>

#include "geometry/detail/debug.hpp"

#include "geometry/constants.hpp"
#include "geometry/euclid_vector.hpp"

namespace geometry
{

using std::numeric_limits;

using std::sqrt;
using std::pow;
using std::sin;
using std::cos;
using std::tan;
using std::asin;
using std::acos;
using std::atan2;

template <typename T>
class euclid_vector_base<T,3,coord::rec>
{
    typedef euclid_vector_base<T,3,coord::rec> thisclass_t;
    typedef euclid_vector<T,3,coord::rec> subclass_t;

  private:
    T _x;
    T _y;
    T _z;

  protected:
    euclid_vector_base()
    {
        DEBUG_CLOG("euclid_vector_base<3,rec> ctor\n")
    };

    euclid_vector_base(const T& x, const T& y, const T& z)
    : _x(x), _y(y), _z(z)
    {
        DEBUG_CLOG("euclid_vector_base<3,rec> ctor(x,y,z)\n")
    }

    // copy constructor
    euclid_vector_base(const thisclass_t& v)
    : _x(v._x), _y(v._y), _z(v._z)
    {
        DEBUG_CLOG("euclid_vector_base<3,rec> copy ctor\n")
    }
    subclass_t& operator=(const thisclass_t& v)
    {
        DEBUG_CLOG("euclid_vector_base<3,rec> operator=\n")
        // This gracefully handles self assignment
        _x = v._x;
        _y = v._y;
        _z = v._z;
        return *dynamic_cast<subclass_t*>(this);
    }

    // type-converting copy constructor
    template <typename U, coord::sys_t C>
    euclid_vector_base(const euclid_vector_base<U,3,C>& v)
    : _x(v.x()), _y(v.y()), _z(v.z())
    {
        DEBUG_CLOG("euclid_vector_base<T,3,rec> copy ctor<U,3,C>\n")
    }
    template <typename U, coord::sys_t C>
    subclass_t& operator=(const euclid_vector_base<U,3,C>& v)
    {
        DEBUG_CLOG("euclid_vector_base<T,3,rec> operator=<U,3,C>\n")
        _x = v.x();
        _y = v.y();
        _z = v.z();
        return *dynamic_cast<subclass_t*>(this);
    }

    virtual
   ~euclid_vector_base() {};

  public:

    // accessor methods
    const T& x() const { return _x; }
    const T& y() const { return _y; }
    const T& z() const { return _z; }


    T r2()  const { return _x*_x + _y*_y + _z*_z; }
    T r()   const { return sqrt(this->r2()); }

    T phi() const { return atan2(_y,_x); }
    T costheta() const
    {
        T l = this->r();
        return (l < (2. * numeric_limits<T>::epsilon())) ? 1.0 : (_z / l);
    }
    T theta() const { return acos(this->costheta()); }

    T rho2() const { return _x*_x + _y*_y; }
    T rho() const { return std::sqrt(this->rho2()); }

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
    subclass_t& z(const T& z)
    {
        _z = z;
        return *dynamic_cast<subclass_t*>(this);
    }
    subclass_t& r(const T& r)
    {
        euclid_vector<T,3,coord::sph> v{r,this->phi(),this->theta()};
        _x = v.x();
        _y = v.y();
        _z = v.z();
        return *dynamic_cast<subclass_t*>(this);
    }
    subclass_t& phi(const T& phi)
    {
        euclid_vector<T,3,coord::sph> v{this->r(),phi,this->theta()};
        _x = v.x();
        _y = v.y();
        return *dynamic_cast<subclass_t*>(this);
    }
    subclass_t& theta(const T& theta)
    {
        euclid_vector<T,3,coord::sph> v{this->r(),this->phi(),theta};
        _x = v.x();
        _y = v.y();
        _z = v.z();
        return *dynamic_cast<subclass_t*>(this);
    }

    template <typename U>
    thisclass_t& rotate_y(U a)
    {
        double ca = cos(a);
        double sa = sin(a);
        this->x( ca * this->x() + sa * this->z());
        this->z(-sa * this->x() + ca * this->z());
        return *this;
    }
};

template <typename T>
class euclid_vector_base<T,3,coord::sph>
{
    typedef euclid_vector_base<T,3,coord::sph> thisclass_t;
    typedef euclid_vector<T,3,coord::sph> subclass_t;

  private:
    T _r;
    T _phi;
    T _theta;

    void validate()
    {
        DEBUG_CLOG("    euclid_vector_base<3,sph>::validate()")
        DEBUG_CLOG(" (" << _r << "," << _phi << "," << _theta <<  ")")

        static const T tol = numeric_limits<T>::epsilon() * 100.;

        if (_r < 0)
        {
            _r = fabs(_r);
            _phi += cons::pi<T>();
            _theta = cons::pi<T>() - _theta;
        }

        // phi in (-pi,pi)
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
        DEBUG_CLOG(" --> (" << _r << "," << _phi << "," << _theta << ")\n")
    }

  protected:
    euclid_vector_base()
    {
        DEBUG_CLOG("euclid_vector_base<3,sph> ctor\n")
    };

    euclid_vector_base(const T& r, const T& phi, const T& theta)
    : _r(r), _phi(phi), _theta(theta)
    {
        DEBUG_CLOG("euclid_vector_base<3,sph> ctor(r,phi)\n")
        this->validate();
    }

    // copy constructor
    euclid_vector_base(const thisclass_t& v)
    : _r(v._r), _phi(v._phi), _theta(v._theta)
    {
        DEBUG_CLOG("euclid_vector_base<3,sph> copy ctor\n")
    }
    subclass_t& operator=(const thisclass_t& v)
    {
        DEBUG_CLOG("euclid_vector_base<3,sph> operator=\n")
        // This gracefully handles self assignment
        _r = v._r;
        _phi = v._phi;
        _theta = v._theta;
        return *dynamic_cast<subclass_t*>(this);
    }

    // type-converting copy constructor
    template <typename U, coord::sys_t C>
    euclid_vector_base(const euclid_vector_base<U,3,C>& v)
    : _r(v.r()), _phi(v.phi()), _theta(v.theta())
    {
        DEBUG_CLOG("euclid_vector_base<T,3,sph> copy ctor<U,3,C>\n")
    }
    template <typename U, coord::sys_t C>
    subclass_t& operator=(const euclid_vector_base<U,3,C>& v)
    {
        DEBUG_CLOG("euclid_vector_base<T,3,sph> operator=<U,3,C>\n")
        _r = v.r();
        _phi = v.phi();
        _theta = v.theta();
        return *dynamic_cast<subclass_t*>(this);
    }

    virtual
   ~euclid_vector_base() {};

  public:

    // accessor methods
    const T& r() const { return _r; }
    const T& phi() const { return _phi; }
    const T& theta() const { return _theta; }

    T rho() const { return _r*sin(_theta); }
    T rho2() const
    {
        T _rho = this->rho();
        return _rho*_rho;
    }

    T x() const { return this->rho()*cos(_phi); }
    T y() const { return this->rho()*sin(_phi); }
    T z() const { return _r*cos(_theta); }

    T r2() const { return _r*_r; }
    T costheta() const { return cos(_theta); }

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
    subclass_t& theta(const T& theta)
    {
        _theta = theta;
        this->validate();
        return *dynamic_cast<subclass_t*>(this);
    }

    subclass_t& x(const T& x)
    {
        euclid_vector<T,3,coord::rec> v{x,this->y(),this->z()};
        _r = v.r();
        _phi = v.phi();
        _theta = v.theta();
        return *dynamic_cast<subclass_t*>(this);
    }
    subclass_t& y(const T& y)
    {
        euclid_vector<T,3,coord::rec> v{this->x(),y,this->z()};
        _r = v.r();
        _phi = v.phi();
        _theta = v.theta();
        return *dynamic_cast<subclass_t*>(this);
    }
    subclass_t& z(const T& z)
    {
        euclid_vector<T,3,coord::rec> v{this->x(),this->y(),z};
        _r = v.r();
        _theta = v.theta();
        return *dynamic_cast<subclass_t*>(this);
    }
};

template <typename T, coord::sys_t coordsys>
class euclid_vector<T,3,coordsys>
: public euclid_vector_base<T,3,coordsys>
, boost::additive< euclid_vector<T,3,coordsys>
, boost::field_operators< euclid_vector<T,3,coordsys>, T
> >
{
    // coordinate type must be one of: rec, sph
    BOOST_STATIC_ASSERT( coordsys & (coord::rec|coord::sph) );

    typedef euclid_vector_base<T,3,coordsys> baseclass_t;
    typedef euclid_vector<T,3,coordsys>      thisclass_t;

  public:
    euclid_vector() { DEBUG_CLOG("euclid_vector<T,3,coordsys> ctor\n") }

    euclid_vector(const T& x0, const T& x1, const T& x2)
    : baseclass_t(x0,x1,x2)
    {
        DEBUG_CLOG("euclid_vector<T,3,coordsys> ctor(x0,x1,x2)\n")
    }
    template <class Vec>
    euclid_vector(const Vec& v)
    {
        DEBUG_CLOG("euclid_vector<T,3> copy ctor<Vec>\n")
        this->x(v.x()).y(v.y()).z(v.z());
    }
    template <class Vec>
    euclid_vector& operator=(const Vec& v)
    {
        DEBUG_CLOG("euclid_vector<T,3> operator=<Vec>\n")
        return this->x(v.x()).y(v.y()).z(v.z());
    }
    // work-around for older compilers
    euclid_vector& operator=(const std::initializer_list<T>& l)
    {
        DEBUG_CLOG("euclid_vector<T,3> operator=<Vec>\n")
        auto val = l.begin();
        return this->x(*val++).y(*val++).z(*val);
    }

    // copy constructor
    euclid_vector(const thisclass_t& v)
    : baseclass_t(v)
    {
        DEBUG_CLOG("euclid_vector<T,3,coordsys> copy ctor\n")
    }
    euclid_vector& operator=(const thisclass_t& v)
    {
        DEBUG_CLOG("euclid_vector<T,3,coordsys> operator=<T,3,coordsys>\n")
        // This gracefully handles self assignment
        baseclass_t::operator=(v);
        return *this;
    }

    // type-converting copy constructor
    template <typename U, coord::sys_t C>
    euclid_vector(const euclid_vector<U,3,C>& v)
    : baseclass_t(v)
    {
        DEBUG_CLOG("euclid_vector<T,3,coordsys> copy ctor<U,3,C>\n")
    }
    template <typename U, coord::sys_t C>
    euclid_vector& operator=(const euclid_vector<U,3,C>& v)
    {
        DEBUG_CLOG("euclid_vector<T,3,coordsys> operator=<U,3,C>\n")
        baseclass_t::operator=(v);
        return *this;
    }

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
    thisclass_t& operator+=(const thisclass_t& v)
    {
        this->x(this->x()+v.x());
        this->y(this->y()+v.y());
        this->z(this->z()+v.z());
        return *this;
    }
    thisclass_t& operator-=(const thisclass_t& v)
    {
        this->x(this->x()-v.x());
        this->y(this->y()-v.y());
        this->z(this->z()-v.z());
        return *this;
    }

    template <typename U, coord::sys_t C>
    thisclass_t operator+(const euclid_vector<U,3,C>& v) const
    {
        thisclass_t vec(*this);
        return vec += v;
    }

    template <typename U, coord::sys_t C>
    thisclass_t operator-(const euclid_vector<U,3,C>& v) const
    {
        thisclass_t vec(*this);
        return vec -= v;
    }

    /** \fn dot_prod(const euclid_vector<U,3,C>& v)
     * v1.dot_prod(v2)  ==  v1 . v2
     **/
    T dot_prod(const thisclass_t& v) const
    {
        long double x2 = this->x()*v.x();
        long double y2 = this->y()*v.y();
        long double z2 = this->z()*v.z();
        return (x2 + y2 + z2);
    }

    T operator*(const thisclass_t& v) const
    {
        return this->dot_prod(v);
    }

    euclid_vector<T,3,coord::rec> cross_prod(const thisclass_t& v) const
    {
        long double zy = this->z() * v.y();
        long double yz = this->y() * v.z();
        long double xz = this->x() * v.z();
        long double zx = this->z() * v.x();
        long double yx = this->y() * v.x();
        long double xy = this->x() * v.y();
        return euclid_vector<T,3,coord::rec>{T(yz-zy), T(zx-xz), T(xy-yx)};
    }

    thisclass_t operator^(const thisclass_t& v) const
    {
        return this->cross_prod(v);
    }

    thisclass_t projection(const thisclass_t& v) const
    {
        return (this->dot_prod(v) / v.r2()) * v;
    }

    T angle(const thisclass_t& v) const
    {
        DEBUG_CLOG("    euclid_vector_base<3,pol>::angle")
        DEBUG_CLOG(" (" << this->r() << "," << this->phi() << "," << this->theta() <<  ")")
        DEBUG_CLOG(", (" << v.r() << "," << v.phi() << "," << v.theta() <<  ")")
        DEBUG_CLOG(", dot_prod: " << this->dot_prod(v))
        DEBUG_CLOG(", a: " << this->r() * v.r())
        DEBUG_CLOG(", angle: " << acos(this->dot_prod(v) / (this->r() * v.r())) << endl)

        static const T tol = numeric_limits<T>::epsilon() * 10.;

        long double a = this->r2() * v.r2();
        a = sqrt(a);
        if (a > 0.)
        {
            long double dotprod = this->dot_prod(v);
            long double cosangle = dotprod / a;
            if (fabs(cosangle) > (1-tol))
            {
                return 0.;
            }
            else
            {
                return acos(cosangle);
            }
        }
        else
        {
            return 0.;
        }
    }

    template <class Vec>
    bool is_parallel(
        const Vec& v,
        const T& tol = std::numeric_limits<T>::epsilon() * 10.
        ) const
    {
        T dphi = abs(this->phi()   - v.phi());
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
};

} /** namespace geometry **/

#endif /** __GEOMETRY_EUCLID_VECTOR3_HPP__ **/
