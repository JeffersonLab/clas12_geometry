/*
#include <cstddef>
#include <string>
#include <vector>

#include "clas12/geometry/central_tracker.hpp"

namespace clas12
{
namespace geometry
{
namespace central_tracker
{

using ::clas12::geometry::CentralTracker;
using forward_svt::Region;


/ **
 * \brief constructor.
 *
 * This forces the FST to have a parent SVT object pointer.
 *
 * \param [in] svt a pointer to the parent CentralTracker class
 ** /
ForwardSVT::ForwardSVT(const CentralTracker* svt)
: _svt(svt)
{
}

/ **
 * \brief copy constructor.
 *
 * This forces the FST to have a parent SVT object pointer.
 *
 * \param [in] that the ForwardSVT being copied
 * \param [in] svt a pointer to the parent CentralTracker class
 ** /
ForwardSVT::ForwardSVT(const ForwardSVT& that, const CentralTracker* svt)
: _svt(svt)
{
    for (size_t i=0; i<that._regions.size(); i++)
    {
        const Region& region = *that._regions[i];
        _regions.emplace_back(new Region(region,this,i));
    }
}

} // namespace clas12::geometry::central_tracker
} // namespace clas12::geometry
} // namespace clas12
*/
