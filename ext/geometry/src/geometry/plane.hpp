#ifndef __GEOMETRY_PLANE_HPP__
#define __GEOMETRY_PLANE_HPP__

#include <cassert>
#include <cmath>
#include <limits>
#include <stdexcept>

#include <boost/assert.hpp>
#include <boost/operators.hpp>

#include "geometry/euclid_vector.hpp"
#include "geometry/direction_vector.hpp"
#include "geometry/line.hpp"
#include "geometry/line_segment.hpp"

namespace geometry {

using std::numeric_limits;
using std::runtime_error;

template <typename T=double>
class plane
{
    BOOST_STATIC_ASSERT( !std::numeric_limits<T>::is_integer );

  private:
    euclid_vector<T,3> _point;
    direction_vector<T,3> _normal;

  public:

    plane() {}
    plane(
        const euclid_vector<T,3>& p,
        const direction_vector<T,3>& n)
    : _point(p), _normal(n)
    {}
    /// the two direction vectors lie in the plane
    /// so the cross product is the normal to the plane.
    plane(
        const euclid_vector<T,3>& p,
        const direction_vector<T,3>& d1,
        const direction_vector<T,3>& d2 )
    : _point(p), _normal(d1^d2)
    {}

    /// assume point is at origin
    plane(const direction_vector<T,3>& n)
    : _point(0.,0.,0.), _normal(n)
    {}

    // copy constructor
    plane(const plane<T>& p)
    : _point(p._point), _normal(p._normal)
    {}
    plane<T>& operator=(const plane<T>& p)
    {
        // This gracefully handles self assignment
        _point = p._point;
        _normal = p._normal;
        return *this;
    }

    // type-converting copy constructor
    template <typename U>
    plane(const plane<U>& p)
    : _point(p.point()), _normal(p.normal())
    {}
    template <typename U>
    plane<T>& operator=(const plane<U>& p)
    {
        _point = p.point();
        _normal = p.normal();
        return *this;
    }

   ~plane() {};

    const euclid_vector<T,3>& point() const
    {
        return _point;
    }
    const direction_vector<T,3>& normal() const
    {
        return _normal;
    }

    plane& point(const euclid_vector<T,3>& v)
    {
        _point = v;
        return *this;
    }
    plane& point(const T& x, const T& y, const T& z)
    {
        _point = {x,y,z};
        return *this;
    }

    plane& normal(const direction_vector<T,3>& dv)
    {
        _normal = dv;
        return *this;
    }
    plane& normal(const euclid_vector<T,3>& v)
    {
        _normal = v;
        return *this;
    }
    plane& normal(const T& phi, const T& theta)
    {
        _normal = {phi,theta};
        return *this;
    }
    plane& normal(const T& x, const T& y, const T& z)
    {
        _normal = {x,y,z};
        return *this;
    }

    template <class Vec>
    bool is_parallel(const Vec& v) const
    {
        static const T tol = numeric_limits<T>::epsilon() * 10.;
        if ((_normal * v) < tol)
        {
            return true;
        }
        return false;
    }
    bool is_parallel(const line<T,3>& l) const
    {
        return this->is_parallel(l.direction());
    }
    bool is_parallel(const line_segment<T,3>& ls) const
    {
        return this->is_parallel(ls.direction());
    }
    bool is_parallel(const plane<T>& p) const
    {
        return _normal.is_parallel(p.normal());
    }

    bool intersects(const line<T,3>& l) const
    {
        return ! this->is_parallel(l);
    }
    bool intersects(const line_segment<T,3>& ls) const
    {
        throw runtime_error("plane.intersects(line_segment) not implemented.");
    }
    bool intersects(plane<T>& p) const
    {
        return ! this->is_parallel(p);
    }

    /**
     * line::intersection(plane)
     *
     * find the point of intersection between a
     * line and a plane
     *
     * lp = line.point()
     * ld = line.direction()
     * pp = plane.point()
     * pn = plane.normal()
     *
     * t = scalar
     *
     *      pn . (pp - lp)
     * t = ----------------
     *          pn . ld
     *
     * intsct = lp + t * ld
     *
     **/
    euclid_vector<T,3> intersection(const line<T,3>& l) const
    {
        T t = _normal * (_point - l.point());
        t  /= _normal * l.direction();
        return (l.point() + t * l.direction());
    }
    euclid_vector<T,3> intersection(const line_segment<T,3>& ls) const
    {
        throw runtime_error("plane.intersection(line_segment) not implemented.");
    }
    line<T,3> intersection(const plane<T>& p) const
    {
        // dir of line
        direction_vector<T,3> dir{_normal ^ p.normal()};

        // line on this plane pointing to plane p
        line<T,3> ln{_point, dir ^ _normal};

        // gets a point on the line of intersection
        euclid_vector<T,3> pnt{p.intersection(ln)};

        return line<T,3>{pnt,dir};
    }

    euclid_vector<T,3> projection(const euclid_vector<T,3>& v) const
    {
        return v - ((v - _point) * _normal) * _normal;
    }
    line<T,3> projection(const line<T,3>& l) const
    {
        direction_vector<T,3> dir{_normal ^ (l.direction() ^ _normal)};
        return line<T,3>{l.intersection(*this), dir};
    }
    line_segment<T,3> projection(const line_segment<T,3>& ls) const
    {
        throw runtime_error("plane.projection(line_segment) not implemented.");
    }

}; // class plane


template <typename T>
line_segment<T,3> shortest_connection(const line<T,3>& l1, const line<T,3>& l2)
{
    // direction from one line to the other
    direction_vector<T,3> axb{l1.direction() ^ l2.direction()};

    // extend line l1 into a plane which contains axb
    // using a point and two direction vectors that lie
    // on the plane
    plane<T> p{l1.point(), l1.direction(), axb};

    // intersection of the other line with this plane
    // gives the point of closest approach of this line
    // with the other line
    euclid_vector<T,3> lp_int = l2.intersection(p);

    // line segment starts with line-plane intersect,
    // goes in the direction of axb for a distance
    // of line-line doca
    return line_segment<T,3>{lp_int, axb, l1.doca(l2)};
}

/**
 * intersection between two lines.
 * always considered skew, takes the midpoint
 * of the shortest line segment between the
 * two lines.
 **/
template <typename T>
euclid_vector<T,3> intersection(const line<T,3>& l1, const line<T,3>& l2)
{
    return shortest_connection(l1,l2).midpoint();
}

template <typename T>
euclid_vector<T,3> intersection(const line<T,3>& l, const plane<T>& p)
{
    return p.intersection(l);
}
template <typename T>
euclid_vector<T,3> intersection(const plane<T>& p, const line<T,3>& l)
{
    return p.intersection(l);
}

template <typename T>
euclid_vector<T,3> projection(const euclid_vector<T,3>& v, const line<T,3>& l)
{
    // plane perpendicular to line, which contains point v
    plane<T> p{v,l.direction()};

    // find intersection of line and plane
    return p.intersection(l);
}
template <typename T>
euclid_vector<T,3> projection(const line<T,3>& l, const euclid_vector<T,3>& v)
{
    return projection(v,l);
}

} // namespace geometry

#endif // __GEOMETRY_PLANE_HPP__
