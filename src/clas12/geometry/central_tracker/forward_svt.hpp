#ifndef CLAS12_GEOMETRY_CENTRAL_TRACKER_FORWARD_SVT_HPP
#define CLAS12_GEOMETRY_CENTRAL_TRACKER_FORWARD_SVT_HPP
/*
#include <iostream>
using std::clog;
using std::endl;

#include <cstddef>
#include <memory>
#include <vector>

#include "forward_svt/region.hpp"

namespace clas12
{
namespace geometry
{

class CentralTracker;

namespace central_tracker
{

using std::unique_ptr;
using std::vector;

using ::clas12::geometry::CentralTracker;
using forward_svt::Region;

/**
 * \brief the forward silicone vertex tracker which consists of several regions
 *
 ** /
class ForwardSVT
{
  public:
    const CentralTracker& svt() const;

    const vector<unique_ptr<Region>>& regions() const;
    const Region& region(const size_t& reg) const;

  private:
    ForwardSVT(const CentralTracker* svt);
    ForwardSVT(const ForwardSVT& that, const CentralTracker* svt);

    /// \brief a pointer to the parent sector
    const CentralTracker* _svt;

    /// \brief a sector consists of several regions
    vector<unique_ptr<Region>> _regions;

    /// \brief deleted copy constructor
    ForwardSVT(const ForwardSVT&) = delete;

    /// \brief deleted assignment operator
    ForwardSVT& operator=(const ForwardSVT&) = delete;

    /// \brief the top-level CentralTracker class has control of all
    /// private members of this class.
    friend class ::clas12::geometry::CentralTracker;
};

/**
 * \brief get the parent CentralTracker object
 * \return constant reference to ForwardSVT::_svt
 ** /
inline const CentralTracker& ForwardSVT::svt() const
{
    return *_svt;
}

/**
 * \brief Get a vector of the regions in this detector
 * \return const reference to ForwardSVT::_regions
 ** /
inline const vector<unique_ptr<Region>>& ForwardSVT::regions() const
{
    return _regions;
}

/**
 * \brief Get a region in this detector
 * \param [in] reg The region index within the FST (counting from zero)
 * \return const reference to ForwardSVT::_regions[reg]
 ** /
inline const Region& ForwardSVT::region(const size_t& reg) const
{
    return *_regions[reg];
}

} // namespace clas12::geometry::central_tracker
} // namespace clas12::geometry
} // namespace clas12

#endif // CLAS12_GEOMETRY_CENTRAL_TRACKER_FORWARD_SVT_HPP
*/
