#ifndef __GEOMETRY_DETAIL_DEBUG_HPP__
#define __GEOMETRY_DETAIL_DEBUG_HPP__

#ifdef GEOMETRY_DEBUG
#include <iostream>
#define DEBUG_CLOG(arg) std::clog << arg;
#else
#define DEBUG_CLOG(arg)
#endif

#endif // __GEOMETRY_DETAIL_DEBUG_HPP__

