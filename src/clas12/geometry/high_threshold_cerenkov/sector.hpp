#ifndef CLAS12_GEOMETRY_HIGH_THRESHOLD_CERENKOV_SECTOR_HPP
#define CLAS12_GEOMETRY_HIGH_THRESHOLD_CERENKOV_SECTOR_HPP

#include <cstddef>
#include <memory>
#include <vector>

#include "geometry.hpp"

#include "mirror.hpp"

namespace clas12
{
namespace geometry
{

class HighThresholdCerenkov;

namespace high_threshold_cerenkov
{

using std::unique_ptr;
using std::vector;

using ::geometry::euclid_vector;
using ::geometry::direction_vector;
using ::geometry::line;
using ::geometry::line_segment;
using ::geometry::plane;

using ::clas12::geometry::HighThresholdCerenkov;

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
    const HighThresholdCerenkov& htcc() const;

    const vector<unique_ptr<Mirror>>& mirrors_left() const;
    const vector<unique_ptr<Mirror>>& mirrors_right() const;
    const Mirror& mirror_left(int m) const;
    const Mirror& mirror_right(int m) const;

    size_t index() const;

    euclid_vector<double,3>    sector_to_clas(const euclid_vector<double,3>& v   ) const;
    direction_vector<double,3> sector_to_clas(const direction_vector<double,3>& v) const;
    line<double,3>             sector_to_clas(const line<double,3>& l            ) const;
    line_segment<double,3>     sector_to_clas(const line_segment<double,3>& l    ) const;
    plane<double>              sector_to_clas(const plane<double>& p             ) const;

  private:
    Sector(const HighThresholdCerenkov* htcc, size_t idx);
    Sector(const Sector& that, const HighThresholdCerenkov* htcc, size_t idx);

    /// \brief a pointer to the parent sector
    const HighThresholdCerenkov* _htcc;

    /// \brief the index of the sector in the vector<Sector>
    /// object held by the HighThresholdCerenkov parent class
    size_t _idx;

    /// \brief a sector consists of several mirrors
    vector<unique_ptr<Mirror>> _mirrors_left;
    vector<unique_ptr<Mirror>> _mirrors_right;

    // private inline methods
    size_t mirror_left_index(int idx) const;
    size_t mirror_right_index(int idx) const;

    /// \brief deleted copy constructor
    Sector(const Sector&) = delete;

    /// \brief deleted assignment operator
    Sector& operator=(const Sector&) = delete;

    /// \brief the top-level DC class has control of all
    /// private members of this class.
    friend class ::clas12::geometry::HighThresholdCerenkov;
};

/**
 * \brief get the parent drift chamber object
 * \return constant reference to Sector::_htcc
 **/
inline const HighThresholdCerenkov& Sector::htcc() const
{
    return *_htcc;
}

/** \fn Sector::mirrors_left()
 * \brief Get a vector of the left mirrors in this sector
 * \return const reference to Sector::_mirrors_left
 **/
inline const vector<unique_ptr<Mirror>>& Sector::mirrors_left() const
{
    return _mirrors_left;
}

/** \fn Sector::mirrors_right()
 * \brief Get a vector of the right mirrors in this sector
 * \return const reference to Sector::_mirrors_right
 **/
inline const vector<unique_ptr<Mirror>>& Sector::mirrors_right() const
{
    return _mirrors_right;
}

/**
 * \brief Get a mirror in this sector from the left side
 * \param [in] reg The mirror index within this sector and side (counting from zero)
 * \return const reference to Sector::_mirror_left[m]
 **/
inline const Mirror& Sector::mirror_left(int m) const
{
    return *_mirrors_left[this->mirror_left_index(m)];
}

/**
 * \brief Get a mirror in this sector from the right side
 * \param [in] reg The mirror index within this sector and side (counting from zero)
 * \return const reference to Sector::_mirror_right[m]
 **/
inline const Mirror& Sector::mirror_right(int m) const
{
    return *_mirrors_right[this->mirror_right_index(m)];
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
 * \return unsigned int index of the mirror in this sector and side
 **/
inline
size_t Sector::mirror_left_index(int idx) const
{
    return idx<0 ? (_mirrors_left.size()+idx) : idx;
}

/**
 * \brief convert negative indexes to positive counting from end
 * \param [in] idx index either from zero or from -1 (counting from end)
 * \return unsigned int index of the mirror in this sector and side
 **/
inline
size_t Sector::mirror_right_index(int idx) const
{
    return idx<0 ? (_mirrors_right.size()+idx) : idx;
}

} // namespace clas12::geometry::high_threshold_cerenkov
} // namespace clas12::geometry
} // namespace clas12

#endif // CLAS12_GEOMETRY_HIGH_THRESHOLD_CERENKOV_SECTOR_HPP
