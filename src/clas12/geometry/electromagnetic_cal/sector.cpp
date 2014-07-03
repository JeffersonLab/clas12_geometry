#include <cstddef>

#include "clas12/geometry/electromagnetic_cal.hpp"

namespace clas12
{
namespace geometry
{
namespace electromagnetic_cal
{

using ::clas12::geometry::ElectromagneticCal;

/**
 * \brief constructor.
 *
 * This forces the sector to have a parent PCal
 * object pointer.
 *
 * \param [in] ec a pointer to the parent ElectromagneticCal class
 * \param [in] idx the index of this sector in vector<Sector>
 *             held by parent ElectromagneticCal.
 **/
Sector::Sector(const ElectromagneticCal* ec, size_t idx)
: _ec(ec)
, _idx(idx)
{}


/**
 * \brief copy constructor.
 *
 * This forces the sector to have a parent EC
 * object pointer.
 *
 * \param [in] that the Sector being copied
 * \param [in] ec a pointer to the parent ElectromagneticCal class
 * \param [in] idx the index of this sector in vector<Sector>
 *             held by parent ElectromagneticCal.
 **/
Sector::Sector(const Sector& that, const ElectromagneticCal* ec, size_t idx)
: _ec(ec)
, _idx(idx)
, _nlayers(that._nlayers)
, _alum_thick(that._alum_thick)
, _thtilt(that._thtilt)
, _dist2tgt(that._dist2tgt)
, _dist2cnt(that._dist2cnt)
, _a1(that._a1)
, _a2(that._a2)
, _a3(that._a3)
, _d2(that._d2)
, _d2prime(that._d2prime)

{
    for (size_t i=0; i<that._layers.size(); i++)
    {
        const Layer& layer = *that._layers[i];
        _layers.emplace_back(new Layer(layer,this,i));
    }
}


euclid_vector<double,3> Sector::sector_to_clas(const euclid_vector<double,3>& v) const
{
    static const double sector_phi = 3.14159 / 3.;
    euclid_vector<double,3> ret = v;
    ret.phi(v.phi() + _idx * sector_phi);
    //clog<< "ROTATE "<<v.phi()<<"  "<<_idx*sector_phi<<endl;
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


} // namespace clas12::geometry::electromagnetic_cal
} // namespace clas12::geometry
} // namespace clas12
