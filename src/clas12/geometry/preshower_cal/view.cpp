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

#include "clas12/geometry/preshower_cal.hpp"

namespace clas12
{
namespace geometry
{
namespace preshower_cal
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
, _scint_width(that._scint_width)
, _scint_max_length(that._scint_max_length)
{}



/**
 * \brief get the name of this view (u, v or w)
 * \return the string "u", "v" or "w"
 **/
//string View::name() const
//{
 //   return _layer->view_name(_idx);
//}



} // namespace clas12::geometry::preshower_cal
} // namespace clas12::geometry
} // namespace clas12
