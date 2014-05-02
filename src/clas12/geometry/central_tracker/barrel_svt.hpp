#ifndef CLAS12_GEOMETRY_CENTRAL_TRACKER_BARREL_SVT_HPP
#define CLAS12_GEOMETRY_CENTRAL_TRACKER_BARREL_SVT_HPP

#include <iostream>
using std::clog;
using std::endl;

#include <cstddef>
#include <memory>
#include <string>
#include <vector>

#include "barrel_svt/region.hpp"

namespace clas12
{
namespace geometry
{

class CentralTracker;

namespace central_tracker
{

using std::unique_ptr;
using std::string;
using std::vector;

using ::clas12::geometry::CentralTracker;
using barrel_svt::Region;

/**
 * \brief the barrel silicone vertex tracker which consists of several regions
 *
 **/
class BarrelSVT
{
  public:
    const CentralTracker& svt() const;

    const vector<unique_ptr<Region>>& regions() const;
    const Region& region(const size_t& reg) const;

    // members in cpp file
    void fetch_nominal_parameters( const string& host,
                                   const string& user,
                                   const string& db );

  private:
    BarrelSVT(const CentralTracker* svt);
    BarrelSVT(const BarrelSVT& that, const CentralTracker* svt);
    BarrelSVT& operator=(const BarrelSVT&);

    /// \brief a pointer to the parent sector
    const CentralTracker* _svt;

    /// \brief a sector consists of several regions
    vector<unique_ptr<Region>> _regions;

    /// \brief deleted copy constructor
    BarrelSVT(const BarrelSVT&) = delete;

    /// \brief the top-level DC class has control of all
    /// private members of this class.
    friend class ::clas12::geometry::CentralTracker;
};

/**
 * \brief get the parent CentralTracker object
 * \return constant reference to BarrelSVT::_svt
 **/
inline const CentralTracker& BarrelSVT::svt() const
{
    return *_svt;
}

/** \fn BarrelSVT::regions()
 * \brief Get a vector of the regions in this detector
 * \return const reference to BarrelSVT::_regions
 **/
inline const vector<unique_ptr<Region>>& BarrelSVT::regions() const
{
    return _regions;
}

/**
 * \brief Get a region in this detector
 * \param [in] reg The region index within the FST (counting from zero)
 * \return const reference to BarrelSVT::_regions[reg]
 **/
inline const Region& BarrelSVT::region(const size_t& reg) const
{
    return *_regions[reg];
}

} // namespace clas12::geometry::central_tracker
} // namespace clas12::geometry
} // namespace clas12

#endif // CLAS12_GEOMETRY_CENTRAL_TRACKER_BARREL_SVT_HPP

