#include <cstddef>

#include "clas12/geometry/central_tracker.hpp"

namespace clas12
{
namespace geometry
{
namespace central_tracker
{
namespace barrel_svt
{

using ::clas12::geometry::central_tracker::BarrelSVT;

/**
 * \brief constructor.
 *
 * This forces the region to have a parent BarrelSVT
 * object pointer.
 *
 * \param [in] bst a pointer to the parent BarrelSVT class
 * \param [in] idx the index of this region in vector<Region>
 *             held by parent BarrelSVT.
 **/
Region::Region(const BarrelSVT* bst, const size_t& idx)
: _bst(bst)
, _idx(idx)
{
}

/**
 * \brief copy constructor.
 *
 * This forces the region to have a parent BarrelSVT
 * object pointer.
 *
 * \param [in] that the Region being copied
 * \param [in] bst a pointer to the parent BarrelSVT class
 * \param [in] idx the index of this region in vector<Region>
 *             held by parent BarrelSVT.
 **/
Region::Region(const Region& that, const BarrelSVT* bst, const size_t& idx)
: _bst(bst)
, _idx(idx)
, _radius(that._radius)
, _zstart(that._zstart)
, _phi(that._phi)
{
    for (size_t i=0; i<that._sectors.size(); i++)
    {
        const Sector& sector = *that._sectors[i];
        _sectors.emplace_back(new Sector(sector,this,i));
    }
}

} /* namespace clas12::geometry::central_tracker::barrel_svt */
} /* namespace clas12::geometry::central_tracker */
} /* namespace clas12::geometry */
} /* namespace clas12 */
