#include <iostream>
using std::clog;
using std::endl;

#include "geometry/io.hpp"


#include <cstddef>
#include <cstdlib>
#include <sstream>
#include <stdexcept>

#include "geometry/euclid_vector.hpp"
#include "geometry/direction_vector.hpp"
#include "geometry/plane.hpp"

#include "clas12/geometry/electromagnetic_cal.hpp"

namespace clas12
{
namespace geometry
{
namespace electromagnetic_cal
{
using std::stringstream;
using std::logic_error;
using std::div;
using ::geometry::euclid_vector;
using ::geometry::direction_vector;
using ::geometry::plane;

/**
 * \brief constructor.
 *
 * This forces the view to have a parent layer
 * object pointer.
 *
 * \param [in] layer a pointer to the parent Layer class
 * \param [in] idx the index of this view in vector<View>
 *             held by parent Layer.
 **/
View::View(const Layer* layer, size_t idx)
: _layer(layer)
, _idx(idx)
{}



/**
 * \brief copy constructor.
 *
 * This forces the view to have a parent layer
 * object pointer.
 *
 * \param [in] that the View being copied
 * \param [in] layer a pointer to the parent Layer class
 * \param [in] idx the index of this view in vector<View>
 *             held by parent Layer.
 **/
View::View(const View& that, const Layer* layer, size_t idx)
: _layer(layer)
, _idx(idx)
, _nstrips(that._nstrips)
, _shift(that._shift)
, _halfwidth(that._halfwidth)
, _deltahw(that._deltahw)
{}



/**
 * \brief get the name of this view (u, v or w)
 * \return the string "u", "v" or "w"
 **/
//string View::name() const
//{
 //   return _layer->view_name(_idx);
//}

/*
double View::strip_length(int s) const
{
    static const double pi     = 3.14159265358979;
    static const double halfpi = 3.14159265358979 /2.;
    double ns = this->nstrips() - s;
    if (this->name() == "u")
    {
        return  _max_length - 2. * ns * _layer->strip_width() * tan(halfpi-_layer->view_angle());
    }
    else // v, w
    {
        return  _max_length - ns * _layer->strip_width() *
                            (tan(halfpi-_layer->view_angle())+tan(2*_layer->view_angle() - halfpi));
    }
}
*/
} // namespace clas12::geometry::electromagnetic_cal
} // namespace clas12::geometry
} // namespace clas12
