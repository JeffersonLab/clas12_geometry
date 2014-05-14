#include <cstddef>
#include <stdexcept>

#include "geometry/euclid_vector.hpp"
#include "geometry/line.hpp"
#include "geometry/line_segment.hpp"
#include "geometry/plane.hpp"

#include "clas12/geometry/high_threshold_cerenkov.hpp"

namespace clas12
{
namespace geometry
{
namespace high_threshold_cerenkov
{

using std::runtime_error;

using ::geometry::euclid_vector;
using ::geometry::line;
using ::geometry::line_segment;
using ::geometry::plane;

/**
 * \brief constructor.
 *
 * This forces the sense layer to have a parent sector
 * object pointer.
 *
 * \param [in] sector a pointer to the parent Sector class
 * \param [in] idx the index of this senselayer in vector<Mirror>
 *             held by parent Sector.
 **/
Mirror::Mirror(const Sector* sector, size_t idx)
: _sector(sector)
, _idx(idx)
{
}

/**
 * \brief copy constructor.
 *
 * This forces the sense layer to have a parent sector
 * object pointer.
 *
 * \param [in] that the Mirror being copied
 * \param [in] sector a pointer to the parent Sector class
 * \param [in] idx the index of this senselayer in vector<Mirror>
 *             held by parent Sector.
 **/
Mirror::Mirror(const Mirror& that, const Sector* sector, size_t idx)
: _sector(sector)
, _idx(idx)
{
}

} // namespace clas12::geometry::high_threshold_cerenkov
} // namespace clas12::geometry
} // namespace clas12
