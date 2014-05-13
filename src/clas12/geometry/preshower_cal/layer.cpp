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
 * This forces the region to have a parent view
 * object pointer.
 *
 * \param [in] view a pointer to the parent View class
 * \param [in] idx the index of this layer in vector<Layer>
 *             held by parent View.
 **/
Layer::Layer(const View* view, size_t idx)
: _view(view)
, _idx(idx)
{}

/**
 * \brief copy constructor.
 *
 * This forces the layer to have a parent view
 * object pointer.
 *
 * \param [in] that the Layer being copied
 * \param [in] view a pointer to the parent View class
 * \param [in] idx the index of this region in vector<Layer>
 *             held by parent View.
 **/
Layer::Layer(const Layer& that, const View* view, size_t idx)
: _view(view)
, _idx(idx)
{}

} // namespace clas12::geometry::preshower_cal
} // namespace clas12::geometry
} // namespace clas12
