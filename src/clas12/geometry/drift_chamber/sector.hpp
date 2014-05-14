#ifndef CLAS12_GEOMETRY_DRIFT_CHAMBER_SECTOR_HPP
#define CLAS12_GEOMETRY_DRIFT_CHAMBER_SECTOR_HPP

#include <iostream>
using std::clog;
using std::endl;

#include <cstddef>
#include <memory>
#include <vector>

#include "geometry.hpp"

#include "region.hpp"

namespace clas12
{
namespace geometry
{

class DriftChamber;

namespace drift_chamber
{

using std::unique_ptr;
using std::vector;

using ::geometry::euclid_vector;
using ::geometry::direction_vector;
using ::geometry::line;
using ::geometry::line_segment;
using ::geometry::plane;

using ::clas12::geometry::DriftChamber;

/**
 * \brief a sector of the drift chamber which consists of several regions
 *
 * There are six sectors of drift chambers in CLAS12 which hold
 * three regions
 **/
class Sector
{
  public:
    // inline methods
    const DriftChamber& dc() const;

    const vector<unique_ptr<Region>>& regions() const;
    const Region& region(int reg) const;

    size_t index() const;

    euclid_vector<double,3>    sector_to_clas(const euclid_vector<double,3>& v   ) const;
    direction_vector<double,3> sector_to_clas(const direction_vector<double,3>& v) const;
    line<double,3>             sector_to_clas(const line<double,3>& l            ) const;
    line_segment<double,3>     sector_to_clas(const line_segment<double,3>& l    ) const;
    plane<double>              sector_to_clas(const plane<double>& p             ) const;

  private:
    Sector(const DriftChamber* dc, size_t idx);
    Sector(const Sector& that, const DriftChamber* dc, size_t idx);

    /// \brief a pointer to the parent sector
    const DriftChamber* _dc;

    /// \brief the index of the sector in the vector<Sector>
    /// object held by the DriftChamber parent class
    size_t _idx;

    /// \brief a sector consists of several regions
    vector<unique_ptr<Region>> _regions;

    // private inline methods
    size_t region_index(int idx) const;

    /// \brief deleted copy constructor
    Sector(const Sector&) = delete;

    /// \brief deleted assignment operator
    Sector& operator=(const Sector&) = delete;

    /// \brief the top-level DC class has control of all
    /// private members of this class.
    friend class ::clas12::geometry::DriftChamber;
};

/**
 * \brief get the parent drift chamber object
 * \return constant reference to Sector::_dc
 **/
inline const DriftChamber& Sector::dc() const
{
    return *_dc;
}

/** \fn Sector::regions()
 * \brief Get a vector of the regions in this sector
 * \return const reference to Sector::_regions
 **/
inline const vector<unique_ptr<Region>>& Sector::regions() const
{
    return _regions;
}

/**
 * \brief Get a region in this sector
 * \param [in] reg The region index within this sector (counting from zero)
 * \return const reference to Sector::_regions[reg]
 **/
inline const Region& Sector::region(int reg) const
{
    return *_regions[this->region_index(reg)];
}

/**
 * \brief Get a index of this sector in the parent region
 * \return const reference to Sector::_idx
 **/
inline
size_t Sector::index() const
{
    return _idx;
}

/**
 * \brief convert negative indexes to positive counting from end
 * \param [in] idx index either from zero or from -1 (counting from end)
 * \return unsigned int index of the region in this sector
 **/
inline
size_t Sector::region_index(int idx) const
{
    return idx<0 ? (_regions.size()+idx) : idx;
}

} // namespace clas12::geometry::drift_chamber
} // namespace clas12::geometry
} // namespace clas12

#endif // CLAS12_GEOMETRY_DRIFT_CHAMBER_SECTOR_HPP
