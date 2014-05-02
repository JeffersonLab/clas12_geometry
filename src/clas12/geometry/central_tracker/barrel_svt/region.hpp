#ifndef CLAS12_GEOMETRY_CENTRAL_TRACKER_BARREL_SVT_REGION_HPP
#define CLAS12_GEOMETRY_CENTRAL_TRACKER_BARREL_SVT_REGION_HPP

#include <iostream>
using std::clog;
using std::endl;

#include <cmath>
#include <cstddef>
#include <memory>
#include <vector>

#include "sector.hpp"

namespace clas12
{
namespace geometry
{

class CentralTracker;

namespace central_tracker
{

class BarrelSVT;

namespace barrel_svt
{

using std::sin;
using std::cos;
using std::unique_ptr;
using std::vector;

using ::clas12::geometry::central_tracker::BarrelSVT;

/**
 * \brief A region of sectors in the barrel SVT (BST)
 **/
class Region
{
  public:
    // inline methods
    const BarrelSVT& bst() const;

    const vector<unique_ptr<Sector>>& sectors() const;
    const Sector& sector(const size_t& sec) const;

    const double& radius() const;
    const double& zstart() const;
    const double& phi() const;

    // methods defined in region.cpp

  private:
    Region(const BarrelSVT* bst, const size_t& idx);
    Region(const Region& that, const BarrelSVT* bst, const size_t& idx);

    /// \brief a pointer to the parent BarrelSVT
    const BarrelSVT* _bst;

    /// \brief the index of the region in the vector<Region>
    /// object held by the BarrelSVT parent class
    size_t _idx;

    /// \brief a region consists of several sectors
    vector<unique_ptr<Sector>> _sectors;

    /// \brief radial distance from the center to the first layer
    /// of each region
    double _radius;

    /// \brief initial z-value for each region
    double _zstart;

    /// \brief rotation angle around z
    double _phi;

    /// \brief deleted copy constructor
    Region(const Region&) = delete;

    /// \brief deleted assignment operator
    Region& operator=(const Region&) = delete;

    /// \brief the top-level SVT class has control of all
    /// private members of this class.
    friend class ::clas12::geometry::CentralTracker;

    /// \brief the "parent" class BarrelSVT takes care of
    /// copying this class when the top-level SVT class's
    /// copy constructor is called.
    friend class ::clas12::geometry::central_tracker::BarrelSVT;
};

/**
 * \brief get the parent BarrelSVT object
 * \return constant reference to Region::_svt
 **/
inline
const BarrelSVT& Region::bst() const
{
    return *_bst;
}

/**
 * \brief Get a vector of sectors in this region
 * \return const reference to Region::_sectors
 **/
inline
const vector<unique_ptr<Sector>>& Region::sectors() const
{
    return _sectors;
}

/**
 * \brief Get a sector in this region
 * \param [in] sec the sector index in this region (starting from zero)
 * \return const reference to Region::_sectors[sec]
 **/
inline
const Sector& Region::sector(const size_t& sec) const
{
    return *_sectors[sec];
}

/**
 * \brief Get the radial distance from the center to the first layer
 * of each region (mm)
 * \return const reference to Region::_radius
 **/
inline
const double& Region::radius() const
{
    return _radius;
}

/**
 * \brief Get the initial z-value for each region (mm)
 * \return const reference to Region::_zstart
 **/
inline
const double& Region::zstart() const
{
    return _zstart;
}

/**
 * \brief Get the rotation angle around z (in radians)
 *
 * This is the angle of the normal vector to the first plane
 * (sector) of this region from the x-axis.
 *
 * \return const reference to Region::_phi
 **/
inline
const double& Region::phi() const
{
    return _phi;
}

} // namespace clas12::geometry::central_tracker::barrel_svt
} // namespace clas12::geometry::central_tracker
} // namespace clas12::geometry
} // namespace clas12

#endif // CLAS12_GEOMETRY_CENTRAL_TRACKER_BARREL_SVT_REGION_HPP

