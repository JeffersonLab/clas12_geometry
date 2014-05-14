#include <cstddef>

#include "clas12/geometry/high_threshold_cerenkov.hpp"

namespace clas12
{
namespace geometry
{
namespace high_threshold_cerenkov
{

using ::clas12::geometry::HighThresholdCerenkov;

/**
 * \brief constructor.
 *
 * This forces the sector to have a parent drift chamber
 * object pointer.
 *
 * \param [in] high_threshold_cerenkov a pointer to the parent HighThresholdCerenkov class
 * \param [in] idx the index of this sector in vector<Sector>
 *             held by parent HighThresholdCerenkov.
 **/
Sector::Sector(const HighThresholdCerenkov* htcc, size_t idx)
: _htcc(htcc)
, _idx(idx)
{
}

/**
 * \brief copy constructor.
 *
 * This forces the sector to have a parent HTCC
 * object pointer.
 *
 * \param [in] that the Sector being copied
 * \param [in] high_threshold_cerenkov a pointer to the parent HighThresholdCerenkov class
 * \param [in] idx the index of this sector in vector<Sector>
 *             held by parent HighThresholdCerenkov.
 **/
Sector::Sector(const Sector& that, const HighThresholdCerenkov* htcc, size_t idx)
: _htcc(htcc)
, _idx(idx)
{
    for (const unique_ptr<Mirror>& mirror : that._mirrors_left)
    {
        _mirrors_left.emplace_back(new Mirror(*mirror,this,mirror->_idx));
    }
    for (const unique_ptr<Mirror>& mirror : that._mirrors_right)
    {
        _mirrors_right.emplace_back(new Mirror(*mirror,this,mirror->_idx));
    }
}

euclid_vector<double,3> Sector::sector_to_clas(const euclid_vector<double,3>& v) const
{
    static const double sector_phi = 3.14159 / 3.;
    euclid_vector<double,3> ret = v;
    ret.phi(v.phi() + _idx * sector_phi);
    return ret;
}

direction_vector<double,3> Sector::sector_to_clas(const direction_vector<double,3>& v) const
{
    static const double sector_phi = 3.14159 / 3.;
    direction_vector<double,3> ret = v;
    ret.phi(v.phi() + _idx * sector_phi);
    return ret;
}

line<double,3> Sector::sector_to_clas(const line<double,3>& l) const
{
    return line<double,3>{
        this->sector_to_clas(l.point()),
        this->sector_to_clas(l.direction()) };
}

line_segment<double,3> Sector::sector_to_clas(const line_segment<double,3>& l) const
{
    return line_segment<double,3>{
        this->sector_to_clas(l.point()),
        this->sector_to_clas(l.direction()) };
}

plane<double> Sector::sector_to_clas(const plane<double>& p) const
{
    return plane<double>{
        this->sector_to_clas(p.point()),
        this->sector_to_clas(p.normal()) };
}

} // namespace clas12::geometry::high_threshold_cerenkov
} // namespace clas12::geometry
} // namespace clas12
