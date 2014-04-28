#ifndef __GEOMETRY_IO_HPP__
#define __GEOMETRY_IO_HPP__

#include <ostream>

#include "geometry.hpp"

template <typename T, geometry::coord::sys_t C>
std::ostream& operator<<(std::ostream& os,
    const geometry::euclid_vector<T,2,C>& c)
{
    os  << '(' << c.x()
        << ", " << c.y()
        << ')';
    return os;
}

template <typename T, geometry::coord::sys_t C>
std::ostream& operator<<(std::ostream& os,
    const geometry::euclid_vector<T,3,C>& c)
{
    os  << '(' << c.x()
        << ", " << c.y()
        << ", " << c.z()
        << ')';
    return os;
}

template <typename T>
std::ostream& operator<<(std::ostream& os,
    const geometry::direction_vector<T,2>& c)
{
    os  << '[' << c.phi() << ']'
        << '(' << c.x()
        << ", " << c.y()
        << ')';
    return os;
}

template <typename T>
std::ostream& operator<<(std::ostream& os,
    const geometry::direction_vector<T,3>& c)
{
    os  << '[' << c.phi()
        << ", " << c.theta() << ']'
        << '(' << c.x()
        << ", " << c.y()
        << ", " << c.z()
        << ')';
    return os;
}

template <typename T, size_t dim>
std::ostream& operator<<(std::ostream& os,
    const geometry::line<T,dim>& c)
{
    os  << "line{" << c.point()
        << ", " << c.direction()
        << "}";
    return os;
}

template <typename T>
std::ostream& operator<<(std::ostream& os,
    const geometry::plane<T>& c)
{
    os  << "plane{" << c.point()
        << "," << c.direction()
        << "}";
    return os;
}

//template <typename T>
//std::ostream& operator<<(std::ostream& os,
    //const geometry::lorentz_vector<T>& c)
//{
    //os  << '(' << c.t()
        //<< ',' << c.x()
        //<< ',' << c.y()
        //<< ',' << c.z() << ')';
    //return os;
//}

#endif /** __GEOMETRY_IO_HPP__ **/
