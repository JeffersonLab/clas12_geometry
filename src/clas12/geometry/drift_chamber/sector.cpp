#include <cstddef>

#include "clas12/geometry/drift_chamber.hpp"

namespace clas12
{
namespace geometry
{
namespace drift_chamber
{

using ::clas12::geometry::DriftChamber;

/**
 * \brief constructor.
 *
 * This forces the sector to have a parent drift chamber
 * object pointer.
 *
 * \param [in] drift_chamber a pointer to the parent DriftChamber class
 * \param [in] idx the index of this sector in vector<Sector>
 *             held by parent DriftChamber.
 **/
Sector::Sector(const DriftChamber* dc, size_t idx)
: _dc(dc)
, _idx(idx)
{
}

/**
 * \brief copy constructor.
 *
 * This forces the sector to have a parent drift chamber
 * object pointer.
 *
 * \param [in] that the Sector being copied
 * \param [in] drift_chamber a pointer to the parent DriftChamber class
 * \param [in] idx the index of this sector in vector<Sector>
 *             held by parent DriftChamber.
 **/
Sector::Sector(const Sector& that, const DriftChamber* dc, size_t idx)
: _dc(dc)
, _idx(idx)
{
    for(const unique_ptr<Region>& region : that._regions)
    {
        _regions.emplace_back(new Region(*region,this,region->_idx));
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


} // namespace clas12::geometry::drift_chamber
} // namespace clas12::geometry
} // namespace clas12
