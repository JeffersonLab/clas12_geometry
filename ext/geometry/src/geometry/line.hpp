#ifndef __GEOMETRY_LINE_HPP__
#define __GEOMETRY_LINE_HPP__

#include <iostream>
using std::clog;
using std::endl;

#include <cassert>
#include <cmath>
#include <stdexcept>

#include <boost/assert.hpp>

#include "geometry/euclid_vector.hpp"
#include "geometry/direction_vector.hpp"

namespace geometry {

using std::abs;
using std::logic_error;
using std::runtime_error;

template <typename T=double, size_t dim=3>
class line
{
    BOOST_STATIC_ASSERT( dim > 1 && dim < 4 );

    BOOST_STATIC_ASSERT( !std::numeric_limits<T>::is_integer );

  protected:
    line() {}
};


template <typename T>
class line<T,2>
{
  protected:
    euclid_vector<T,2> _point;
    direction_vector<T,2> _direction;

  public:
    line() {}
    line(const euclid_vector<T,2>& pt, const direction_vector<T,2>& dr)
    : _point(pt), _direction(dr)
    {}
    line(const T& x, const T& y, const T& phi)
    : _point(x,y), _direction(phi)
    {}
    line(const T& pt_x, const T& pt_y,
         const T& dr_x, const T& dr_y )
    : _point(pt_x,pt_y), _direction(dr_x,dr_y)
    {}

    // copy constructor
    line(const line<T,2>& l)
    : _point(l._point), _direction(l._direction)
    {}
    line<T,2>& operator=(const line<T,2>& l)
    {
        // This gracefully handles self assignment
        _point = l._point;
        _direction = l._direction;
        return *this;
    }

    // type-converting copy constructor
    template <typename U>
    line(const line<U,2>& l)
    : _point(l.point()), _direction(l.direction())
    {}
    template <typename U>
    line<T,2>& operator=(const line<U,2>& l)
    {
        _point = l.point();
        _direction = l.direction();
        return *this;
    }

    virtual
   ~line() {};

    const euclid_vector<T,2>& point() const
    {
        return _point;
    }
    const direction_vector<T,2>& direction() const
    {
        return _direction;
    }

    line& point(const T& x, const T& y)
    {
        this->_point = {x,y};
        return *this;
    }
    template <class Vec>
    line& point(const Vec& v)
    {
        _point = v;
        return *this;
    }

    line& direction(const T& phi)
    {
        this->_direction = {phi};
        return *this;
    }
    template <typename U>
    line& direction(const direction_vector<U,2>& v)
    {
        this->_direction = v;
        return *this;
    }
    template <typename U, coord::sys_t C>
    line& direction(const euclid_vector<U,2,C>& v)
    {
        this->_direction = v;
        return *this;
    }

    template <class Vec>
    T angle(const Vec& v) const
    {
        return _direction.angle(v);
    }
    template <typename U>
    T angle(const line<U,2>& l) const
    {
        return _direction.angle(l.direction());
    }

    template <class Vec>
    bool is_parallel(const Vec& v) const
    {
        return _direction.is_parallel(v);
    }
    template <typename U>
    bool is_parallel(const line<U,2>& l) const
    {
        return _direction.is_parallel(l.direction());
    }

    bool intersects(const line<T,2>& l) const
    {
        return ! this->is_parallel(l);
    }

    template <class Vec>
    T doca(const Vec& v) const
    {
        return ((_point-v)-((_point-v)*_direction)*_direction).r();
    }
    template <typename U>
    T doca(const line<U,2>& l) const
    {
        if (this->is_parallel(l))
        {
            return this->doca(l.point());
        }
        return 0;
    }

    /**
     * intersection between two lines.
     **/
    euclid_vector<T,2> intersection(const line<T,2>& l) const
    {
        long double x1 = _point.x();
        long double y1 = _point.y();
        euclid_vector<long double,2> p2{_point + _direction};
        long double x2 = p2.x();
        long double y2 = p2.y();

        long double x3 = l.point().x();
        long double y3 = l.point().y();
        euclid_vector<long double,2> p4{l.point() + l.direction()};
        long double x4 = p4.x();
        long double y4 = p4.y();

        long double det = (x1-x2)*(y3-y4) - (y1-y2)*(x3-x4);

        if (det == 0)
        {
            throw logic_error("line<2>.intersect(line): lines are parallel.\n");
        }

        long double a = x1*y2 - y1*x2;
        long double b = x3*y4 - y3*x4;

        long double x0 = a * (x3 - x4) - b * (x1 - x2);
        long double y0 = a * (y3 - y4) - b * (y1 - y2);

        return euclid_vector<T,2>{x0/det, y0/det};
    }

    euclid_vector<T,2> projection(const euclid_vector<T,2>& v) const
    {
        direction_vector<T,2> perp{_direction.phi() + cons::half_pi<T>()};
        return this->intersection(line<T,2>{v, perp});
    }
};


template <typename T>
class line<T,3>
{
  protected:
    euclid_vector<T,3> _point;
    direction_vector<T,3> _direction;

  public:
    line() {}
    line(const euclid_vector<T,3>& pt, const direction_vector<T,3>& dr)
    : _point(pt), _direction(dr)
    {}
    line(const T& x, const T& y, const T& z,
         const T& phi, const T& theta)
    : _point(x,y,z), _direction(phi,theta)
    {}
    line(const T& pt_x, const T& pt_y, const T& pt_z,
         const T& dr_x, const T& dr_y, const T& dr_z)
    : _point(pt_x,pt_y,pt_z), _direction(dr_x,dr_y,dr_z)
    {}

    // copy constructor
    line(const line<T,3>& l)
    : _point(l._point), _direction(l._direction)
    {}
    line<T,3>& operator=(const line<T,3>& l)
    {
        // This gracefully handles self assignment
        _point = l._point;
        _direction = l._direction;
        return *this;
    }

    // type-converting copy constructor
    template <typename U>
    line(const line<U,3>& l)
    : _point(l.point()), _direction(l.direction())
    {}
    template <typename U>
    line<T,3>& operator=(const line<U,3>& l)
    {
        _point = l.point();
        _direction = l.direction();
        return *this;
    }

   ~line() {};

    const euclid_vector<T,3>& point() const
    {
        return _point;
    }
    const direction_vector<T,3>& direction() const
    {
        return _direction;
    }

    line& point(const T& x, const T& y, const T& z)
    {
        _point = {x,y,z};
        return *this;
    }
    template <class Vec>
    line& point(const Vec& v)
    {
        _point = v;
        return *this;
    }

    line& direction(const T& phi, const T& theta)
    {
        _direction = {phi,theta};
        return *this;
    }
    line& direction(const T& x, const T& y, const T& z)
    {
        _direction = {x,y,z};
        return *this;
    }
    template <class Vec>
    line& direction(const Vec& v)
    {
        _direction = v;
        return *this;
    }

    template <class Vec>
    bool angle(const Vec& v) const
    {
        return _direction.angle(v);
    }
    template <typename U>
    bool angle(const line<U,3>& l) const
    {
        return _direction.angle(l.direction());
    }

    template <class Vec>
    bool is_parallel(const Vec& v) const
    {
        return _direction.is_parallel(v);
    }
    template <typename U>
    bool is_parallel(const line<U,3>& l) const
    {
        return _direction.is_parallel(l.direction());
    }

    virtual
    bool intersects(const line<T,3>& l) const
    {
        return ! _direction.is_parallel(l.direction());
    }

    /**
     * doca to a point
     **/
    template <class Vec>
    T doca(const Vec& v) const
    {
        return ((_point-v)-((_point-v)*_direction)*_direction).r();
    }
    /**
     * distance of closest approach to another line
     *
     * x1      = this->point()
     * x2 - x1 = this->direction()
     * x3      = l.point()
     * x4 - x3 = l.direction()
     *
     * a = x2 - x1 = this->direction()
     * b = x4 - x3 = l.direction()
     * c = x3 - x1 = l.point() - this->point()
     *
     * DOCA = |c . (a x b)| / ||a x b||
     *   a and b are unit vectors, so ||a x b|| = 1, and
     * DOCA = |c . (a x b)|
     **/
    template <typename U>
    T doca(const line<U,3>& l) const
    {
        euclid_vector<T,3> c = l.point() - _point;

        if (this->is_parallel(l))
        {
            return c.r() * cos(c.angle(l.direction()));
        }

        direction_vector<T,3> axb;
        axb = _direction ^ l.direction();
        return abs(c * axb);
    }

    virtual
    euclid_vector<T,3> projection(const euclid_vector<T,3>& v) const
    {
        throw runtime_error("line<3>.projection(euclid_vector<3>) not implemented.");
    }
};

template <typename T, size_t dim>
bool angle(const line<T,dim>& l, const euclid_vector<T,dim>& v)
{
    return l.angle(v);
}
template <typename T, size_t dim>
bool angle(const euclid_vector<T,dim>& v, const line<T,dim>& l)
{
    return l.angle(v);
}
template <typename T, size_t dim>
bool angle(const line<T,dim>& l1, const line<T,dim>& l2)
{
    return l1.angle(l2);
}

template <typename T, size_t dim>
bool is_parallel(const line<T,dim>& l, const euclid_vector<T,dim>& v)
{
    return l.is_parallel(v);
}
template <typename T, size_t dim>
bool is_parallel(const euclid_vector<T,dim>& v, const line<T,dim>& l)
{
    return l.is_parallel(v);
}
template <typename T, size_t dim>
bool is_parallel(const line<T,dim>& l1, const line<T,dim>& l2)
{
    return l1.is_parallel(l2);
}

template <typename T, size_t dim>
bool intersects(const line<T,dim>& l1, const line<T,dim>& l2)
{
    return l1.intersects(l2);
}

template <typename T, size_t dim>
T doca(const line<T,dim>& l, const euclid_vector<T,dim>& v)
{
    return l.doca(v);
}
template <typename T, size_t dim>
T doca(const euclid_vector<T,dim>& v, const line<T,dim>& l)
{
    return l.doca(v);
}
template <typename T>
T doca(const line<T,3>& l1, const line<T,3>& l2)
{
    return l1.doca(l2);
}

template <typename T>
euclid_vector<T,2> projection(const line<T,2>& l, const euclid_vector<T,2>& v)
{
    return l.projection(v);
}
template <typename T>
euclid_vector<T,2> projection(const euclid_vector<T,2>& v, const line<T,2>& l)
{
    return l.projection(v);
}

} // namespace geometry

#endif // __GEOMETRY_LINE_HPP__
