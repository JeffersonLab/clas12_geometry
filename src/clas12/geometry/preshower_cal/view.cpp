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
 * This forces the region to have a parent sector
 * object pointer.
 *
 * \param [in] sector a pointer to the parent Sector class
 * \param [in] idx the index of this view in vector<View>
 *             held by parent Sector.
 **/
View::View(const Sector* sector, size_t idx)
: _sector(sector)
, _idx(idx)
{}

/**
 * \brief copy constructor.
 *
 * This forces the view to have a parent sector
 * object pointer.
 *
 * \param [in] that the View being copied
 * \param [in] sector a pointer to the parent Sector class
 * \param [in] idx the index of this region in vector<View>
 *             held by parent Sector.
 **/
View::View(const View& that, const Sector* sector, size_t idx)
: _sector(sector)
, _idx(idx)
{
    for (size_t i=0; i<that._layers.size(); i++)
    {
        const Layer& lyr = *that._layers[i];
        _layers.emplace_back(new Layer(lyr,this,i));
    }
}

/**
 * \brief get the name of this view (u, v or w)
 * \return the string "u", "v" or "w"
 **/
string View::name() const
{
    return _sector->view_name(_idx);
}

} // namespace clas12::geometry::preshower_cal
} // namespace clas12::geometry
} // namespace clas12
