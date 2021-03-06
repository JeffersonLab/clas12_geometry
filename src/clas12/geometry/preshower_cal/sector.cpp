#include <cstddef>

#include "clas12/geometry/preshower_cal.hpp"

namespace clas12
{
namespace geometry
{
namespace preshower_cal
{

using ::clas12::geometry::PreshowerCal;

/**
 * \brief constructor.
 *
 * This forces the sector to have a parent PCal
 * object pointer.
 *
 * \param [in] pcal a pointer to the parent PreshowerCal class
 * \param [in] idx the index of this sector in vector<Sector>
 *             held by parent PreshowerCal.
 **/
Sector::Sector(const PreshowerCal* pcal, size_t idx)
: _pcal(pcal)
, _idx(idx)
{}


/**
 * \brief copy constructor.
 *
 * This forces the sector to have a parent PCal
 * object pointer.
 *
 * \param [in] that the Sector being copied
 * \param [in] pcal a pointer to the parent PreshowerCal class
 * \param [in] idx the index of this sector in vector<Sector>
 *             held by parent PreshowerCal.
 **/
Sector::Sector(const Sector& that, const PreshowerCal* pcal, size_t idx)
: _pcal(pcal)
, _idx(idx)
, _nlayers(that._nlayers)
, _nfoam(that._nfoam)
, _nsteel(that._nsteel)
, _foam_thick(that._foam_thick)
, _steel_thick(that._steel_thick)
, _thtilt(that._thtilt)
,_dist2tgt(that._dist2tgt)
,_yhigh(that._yhigh)
{
    for (size_t i=0; i<that._layers.size(); i++)
    {
        const Layer& layer = *that._layers[i];
        _layers.emplace_back(new Layer(layer,this,i));
    }
}

euclid_vector<double,3> Sector::pcal_to_sector(const euclid_vector<double,3>& v) const
{
    static const double alpha = this->thtilt();
    const double x0 = this->dist2tgt()*sin(alpha);
    const double y0 = 0.0;
    const double z0 = this->dist2tgt()*cos(alpha);
    double x = x0 + v.y()*cos(alpha) + v.z()*sin(alpha);
    double y = y0 + v.x();
    double z = z0 - v.y()*sin(alpha) + v.z()*cos(alpha);
    euclid_vector<double,3> ret = {x,y,z};
    return ret;
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


} // namespace clas12::geometry::preshower_cal
} // namespace clas12::geometry
} // namespace clas12
