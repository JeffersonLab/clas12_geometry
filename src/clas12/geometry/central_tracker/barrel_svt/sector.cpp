#include <cstddef>

#include "geometry/constants.hpp"
#include "geometry/direction_vector.hpp"
#include "geometry/euclid_vector.hpp"
#include "geometry/plane.hpp"

#include "clas12/geometry/central_tracker.hpp"

namespace clas12
{
namespace geometry
{
namespace central_tracker
{
namespace barrel_svt
{

using namespace ::geometry;

/**
 * \brief constructor.
 *
 * This forces the sector to have a parent Region
 * object pointer.
 *
 * \param [in] region a pointer to the parent Region class
 * \param [in] idx the index of this sector in vector<Sector>
 *             held by parent Region.
 **/
Sector::Sector(const Region* region, const size_t& idx)
: _region(region)
, _idx(idx)
{
}

/**
 * \brief copy constructor.
 *
 * This forces the sector to have a parent Region
 * object pointer.
 *
 * \param [in] that the Sector being copied
 * \param [in] region a pointer to the parent Region class
 * \param [in] idx the index of this sector in vector<Sector>
 *             held by parent Region.
 **/
Sector::Sector(const Sector& that, const Region* region, const size_t& idx)
: _region(region)
, _idx(idx)
, _layergap(that._layergap)
, _fillerthick(that._fillerthick)
{
    for (size_t i=0; i<that._layers.size(); i++)
    {
        const Layer& layer = *that._layers[i];
        _layers.emplace_back(new Layer(layer,this,i));
    }
}


/**
 * \brief this module's 3D plane
 * \return plane<double> with point and normal to the module-plane
 **/
plane<double> Sector::detector_plane() const
{
    double phi = this->phi();
    double theta = 0.5 * cons::pi<double>();

    return plane<double>{
        euclid_vector<double,3,coord::sph>{_region->radius(), phi, theta},
        direction_vector<double,3>{phi, theta}
    };
}

/**
 * \brief this sector's phi angle - the angle of the normal vector to the
 * plane of this sector from the x-axis
 * \return const reference to sector_phi
 **/
double Sector::phi() const
{
    double phi = (2. * cons::pi<double>() / _region->sectors().size()) * _idx;
    phi += _region->phi();
    return phi;
}



} /* namespace clas12::geometry::central_tracker::barrel_svt */
} /* namespace clas12::geometry::central_tracker */
} /* namespace clas12::geometry */
} /* namespace clas12 */
