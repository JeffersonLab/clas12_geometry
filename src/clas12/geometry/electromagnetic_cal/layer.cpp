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
 * This forces the layer to have a parent sector
 * object pointer.
 *
 * \param [in] sector a pointer to the parent Sector class
 * \param [in] idx the index of this layer in vector<Layer>
 *             held by parent Sector.
 **/
Layer::Layer(const Sector* sector, size_t idx)
: _sector(sector)
, _idx(idx)
{}

/**
 * \brief copy constructor.
 *
 * This forces the layer to have a parent sector
 * object pointer.
 *
 * \param [in] that the Layer being copied
 * \param [in] sector a pointer to the parent Sector class
 * \param [in] idx the index of this layer in vector<Layer>
 *             held by parent Sector.
 **/
Layer::Layer(const Layer& that, const Sector* sector, size_t idx)
: _sector(sector)
, _idx(idx)
, _nviews(that._nviews)
, _view_angle(that._view_angle)
, _wrapper_thick(that._wrapper_thick)
, _strip_thick(that._strip_thick)
, _lead_thick(that._lead_thick)

{
    for (size_t i=0; i<that._views.size(); i++)
    {
        const View& iview = *that._views[i];
        _views.emplace_back(new View(iview,this,i));
    }
}


/**
 * \brief get the name of this view (u, v or w)
 * \return the string "u", "v" or "w"
 **/
string View::name() const
{
    return _layer->view_name(_idx);
}


} // namespace clas12::geometry::electromagnetic_cal
} // namespace clas12::geometry
} // namespace clas12
