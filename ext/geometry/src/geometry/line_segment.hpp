#ifndef __GEOMETRY_LINE_SEGMENT_HPP__
#define __GEOMETRY_LINE_SEGMENT_HPP__

#include <cassert>
#include <cmath>
#include <stdexcept>

#include <boost/assert.hpp>

#include "geometry/euclid_vector.hpp"
#include "geometry/direction_vector.hpp"
#include "geometry/line.hpp"

namespace geometry {

using std::abs;
using std::runtime_error;

template <typename T=double, size_t dim=3>
class line_segment : public line<T,dim>
{
    BOOST_STATIC_ASSERT( dim > 1 && dim < 4 );

    BOOST_STATIC_ASSERT( !std::numeric_limits<T>::is_integer );

  protected:
    using line<T,dim>::_point;
    using line<T,dim>::_direction;
    T _length;

  public:
    line_segment() {}
    line_segment(const line<T,dim>& l, const T& len)
    : line<T,dim>(l.point(),l.direction()), _length(len)
    {}
    line_segment(
        const euclid_vector<T,dim>& pt,
        const direction_vector<T,dim>& dr,
        const T& len )
    : line<T,dim>(pt,dr), _length(len)
    {}
    line_segment(
        const euclid_vector<T,dim>& pt1,
        const euclid_vector<T,dim>& pt2 )
    : line<T,dim>(pt1,pt2-pt1), _length((pt2-pt1).r())
    {}

    // copy constructor
    line_segment(const line_segment<T,dim>& ls)
    : line<T,dim>(ls._point,ls._direction), _length(ls._length)
    {}
    line_segment<T,dim>& operator=(const line_segment<T,dim>& ls)
    {
        // This gracefully handles self assignment
        _point = ls._point;
        _direction = ls._direction;
        _length = ls._length;
        return *this;
    }

    // type-converting copy constructor
    template <typename U>
    line_segment(const line_segment<U,dim>& ls)
    : line<T,dim>(ls.point(),ls.direction()), _length(ls.length())
    {}
    template <typename U>
    line_segment<T,dim>& operator=(const line_segment<U,dim>& ls)
    {
        _point = ls.point();
        _direction = ls.direction();
        _length = ls.length();
        return *this;
    }

   ~line_segment() {};

    const T& length() const { return _length; }

    line_segment& length(const T& l)
    {
        _length = l;
        return *this;
    }

    const euclid_vector<T,dim>& begin_point() const
    {
        return _point;
    }
    euclid_vector<T,dim> mid_point() const
    {
        return _point + (0.5*_length) * _direction;
    }
    euclid_vector<T,dim> end_point() const
    {
        return _point + _length * _direction;
    }

    template <class Vec>
    T angle(const Vec& v) const
    {
        return _direction.angle(v);
    }
    T angle(const line<T,dim>& l) const
    {
        return _direction.angle(l.direction());
    }
    T angle(const line_segment<T,dim>& ls) const
    {
        return _direction.angle(ls.direction());
    }

    template <class Vec>
    bool is_parallel(const Vec& v) const
    {
        return _direction.is_parallel(v);
    }
    bool is_parallel(const line<T,3>& l) const
    {
        return _direction.is_parallel(l.direction());
    }
    bool is_parallel(const line_segment<T,3>& ls) const
    {
        return _direction.is_parallel(ls.direction());
    }

    bool intersects(const line<T,dim>& l) const
    {
        throw runtime_error("line_segment.intersects(line) not implemented.");
    }
    bool intersects(const line_segment<T,dim>& ls) const
    {
        throw runtime_error("line_segment.intersects(line_segment) not implemented.");
    }

    T doca(const line<T,dim>& l) const
    {
        throw runtime_error("line_segment.doca(line) not implemented.");
    }
    T doca(const line_segment<T,dim>& ls) const
    {
        throw runtime_error("line_segment.doca(line_segment) not implemented.");
    }

    line_segment<T,dim> projection(const line<T,dim>& l) const
    {
        throw runtime_error("line_segment.projection(line) not implemented.");
    }
    line_segment<T,dim> projection(const line_segment<T,dim>& ls) const
    {
        throw runtime_error("line_segment.projection(line_segment) not implemented.");
    }
};

} // namespace geometry

#endif // __GEOMETRY_LINE_SEGMENT_HPP__
