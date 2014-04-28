#ifndef CLAS12_GEOMETRY_DRIFT_CHAMBER_REGION_HPP
#define CLAS12_GEOMETRY_DRIFT_CHAMBER_REGION_HPP

#include <iostream>
using std::clog;
using std::endl;

#include <cmath>
#include <cstddef>
#include <memory>
#include <vector>

#include "geometry/plane.hpp"

#include "clas12/geometry/coordsys.hpp"
#include "superlayer.hpp"

namespace clas12
{
namespace geometry
{

class DriftChamber;

namespace drift_chamber
{

using std::sin;
using std::cos;
using std::unique_ptr;
using std::vector;

using ::geometry::plane;

class Sector;

/** \class Region
 * \brief A region of superlayers in a sector of the drift chambers
 *
 * There are three regions in each sector of CLAS12
 **/
class Region
{
  public:
    // inline methods
    const Sector& sector() const;

    const vector<unique_ptr<Superlayer>>& superlayers() const;
    const Superlayer& superlayer(const int& slyr) const;

    const double& dist2tgt() const;
    const double& frontgap() const;
    const double& midgap() const;
    const double& backgap() const;
    const double& thopen() const;
    const double& thtilt() const;
    const double& xdist() const;

    // methods in cpp file
    double thickness() const;

    plane<double> left_end_plate(coordsys_t coordsys=COORDSYS::SECTOR) const;
    plane<double> right_end_plate(coordsys_t coordsys=COORDSYS::SECTOR) const;

    euclid_vector<double,3> center(coordsys_t coordsys=COORDSYS::SECTOR) const;

  private:
    Region(const Sector* sector, const size_t& idx);
    Region(const Region& that, const Sector* sector, const size_t& idx);

    /// \brief a pointer to the parent sector
    const Sector* _sector;

    /// \brief the index of the region in the vector<Region>
    /// object held by the Sector parent class
    size_t _idx;

    /// \brief a region consists of several super layers
    vector<unique_ptr<Superlayer>> _superlayers;

    /// \brief distance from the target to the first guard wire plane
    /// along the normal of said plane
    double _dist2tgt;

    /// \brief distance between the upstream gas bag and the first guard wire layer
    double _frontgap;

    /// \brief the distance between the last guard wire layer of one
    /// superlayer and the first guard wire layer of the next superlayer
    double _midgap;

    /// \brief distance between the last guard wire layer of a
    /// region and the downstream gas bag
    double _backgap;

    /// \brief opening angle between endplate planes (radians)
    double _thopen;

    /// \brief tilt angle (relative to z) of the six superlayers (radians)
    double _thtilt;

    /// \brief distance between the line of intersection of the two
    /// endplate planes and the target position
    double _xdist;

    // private inline methods
    size_t superlayer_index(const int& idx) const;

    /// \brief deleted copy constructor
    Region(const Region&) = delete;

    /// \brief deleted assignment operator
    Region& operator=(const Region&) = delete;

    /// \brief the top-level DC class has control of all
    /// private members of this class.
    friend class ::clas12::geometry::DriftChamber;

    /// \brief the "parent" class Sector takes care of
    /// copying this class when the top-level DC class's
    /// copy constructor is called.
    friend class Sector;
};

/**
 * \brief get the parent sector object
 * \return constant reference to Region::_sector
 **/
inline
const Sector& Region::sector() const
{
    return *_sector;
}

/**
 * \brief Get a vector of superlayers in this region
 * \return const reference to Region::_superlayers
 **/
inline
const vector<unique_ptr<Superlayer>>& Region::superlayers() const
{
    return _superlayers;
}

/**
 * \brief Get a superlayer in this region
 * \param [in] slyr the superlayer index in this region (starting from zero)
 * \return const reference to Region::_superlayers[slyr]
 **/
inline
const Superlayer& Region::superlayer(const int& slyr) const
{
    return *_superlayers[this->superlayer_index(slyr)];
}

/**
 * \brief Get distance from target center to the first guard layer
 *
 * This is the distance from the nominal CLAS12 center to
 * the first (guard) wire-plane of the closest superlayer.
 *
 * \return const reference to Region::_dist2tgt
 **/
inline
const double& Region::dist2tgt() const
{
    return _dist2tgt;
}

/**
 * \brief Get distance from the first (guard) wire-plane to upstream gas bag
 * \return const reference to Region::_frontgap
 **/
inline
const double& Region::frontgap() const
{
    return _frontgap;
}

/**
 * \brief Get the gap between superlayers
 *
 * This is degfined as the distance between the last guard wire layer
 * of one superlayer and the first guard wire layer of the next
 * superlayer in this region.
 *
 * \return const reference to Region::_midgap
 **/
inline
const double& Region::midgap() const
{
    return _midgap;
}

/**
 * \brief Get distance from the last (guard) wire-plane to downstream gas bag
 * \return const reference to Region::_backgap
 **/
inline
const double& Region::backgap() const
{
    return _backgap;
}

/**
 * \brief Get the opening angle between endplate planes
 * \return const reference to Region::_thopen (radians)
 **/
inline
const double& Region::thopen() const
{
    return _thopen;
}

/**
 * \brief Get the tilt angle (relative to z) of the six superlayers
 *
 * This is the angle between the z-axis and the normal to
 * the wire-planes.
 *
 * \return const reference to Region::_thtilt (radians)
 **/
inline
const double& Region::thtilt() const
{
    return _thtilt;
}

/**
 * \brief Get the "xdist" of the endplate planes from the target
 *
 * This is the distance between the line of intersection
 * of the two endplate planes and the target position
 *
 * \return const reference to Region::_xdist
 **/
inline
const double& Region::xdist() const
{
    return _xdist;
}

/**
 * \brief convert negative indexes to positive counting from end
 * \param [in] idx index either from zero or from -1 (counting from end)
 * \return unsigned int index of the superlayer in this region
 **/
inline
size_t Region::superlayer_index(const int& idx) const
{
    return idx<0 ? (_superlayers.size()+idx) : idx;
}

} // namespace clas12::geometry::drift_chamber
} // namespace clas12::geometry
} // namespace clas12

#endif // CLAS12_GEOMETRY_DRIFT_CHAMBER_REGION_HPP
