#include <iostream>
using std::clog;
using std::endl;

#include <cmath>
#include <cstddef>
#include <stdexcept>

#include "geometry/constants.hpp"
#include "geometry/direction_vector.hpp"

#include "clas12/geometry/drift_chamber.hpp"

namespace clas12
{
namespace geometry
{
namespace drift_chamber
{

using std::runtime_error;

using std::cos;
using std::sin;

using ::geometry::direction_vector;

namespace cons = ::geometry::cons;

/**
 * \brief constructor.
 *
 * This forces the superlayer to have a parent region
 * object pointer.
 *
 * \param [in] region a pointer to the parent Region class
 * \param [in] idx the index of this superlayer in vector<Superlayer>
 *             held by parent Region.
 **/
Superlayer::Superlayer(const Region* region, size_t idx)
: _region(region)
, _idx(idx)
{
}

/**
 * \brief copy constructor.
 *
 * This forces the superlayer to have a parent region
 * object pointer.
 *
 * \param [in] that the Superlayer being copied
 * \param [in] region a pointer to the parent Region class
 * \param [in] idx the index of this superlayer in vector<Superlayer>
 *             held by parent Region.
 **/
Superlayer::Superlayer(const Superlayer& that, const Region* region, size_t idx)
: _region(region)
, _idx(idx)
, _nfieldlayers(that._nfieldlayers)
, _thmin(that._thmin)
, _thster(that._thster)
, _wpdist(that._wpdist)
, _cellthickness(that._cellthickness)
{
    for (size_t i=0; i<that._senselayers.size(); i++)
    {
        const Senselayer& senselayer = *that._senselayers[i];
        _senselayers.emplace_back(new Senselayer(senselayer,this,i));
    }
    for (size_t i=0; i<that._guardlayers.size(); i++)
    {
        const Guardlayer& guardlayer = *that._guardlayers[i];
        _guardlayers.emplace_back(new Guardlayer(guardlayer,this,i));
    }
}

/**
 * \brief Get the total number of wire-planes
 *
 * The total number of wire-planes in this superlayer consists of
 * sense and guard layers, and between each of these, there
 * are nfieldlayers number of field wire planes.
 *
 * \return total number of wire-planes
 **/
size_t Superlayer::nwireplanes() const
{
    size_t nwplanes = _senselayers.size() + _guardlayers.size();
    nwplanes += (nwplanes-1) * _nfieldlayers;
    return nwplanes;
}

/**
 * \brief Get the total number of layers (guard and sense wire-planes)
 *
 * The total number of layers in this superlayer consists of
 * sense and guard layers.
 *
 * \return total number of layers
 **/
size_t Superlayer::nlayers() const
{
    return _senselayers.size() + _guardlayers.size();
}

/**
 * \brief Get the total thickness of this superlayer
 *
 * This is the distance between the first and last wire plane.
 *
 * \return thickness
 **/
double Superlayer::thickness() const
{
    return (this->nlayers() - 1) * this->layer_thickness();
}

/**
 * \brief Get distance from target center to the first guard layer
 *
 * This is the distance from the nominal CLAS12 center to
 * the first (guard) wire-plane of this superlayer.
 *
 * \return distance (cm)
 **/
double Superlayer::dist2tgt() const
{
    // start with distance from target to the
    // first guard wire plane in the region
    // This value is returned if we are on the first superlayer
    double dist2tgt = _region->dist2tgt();

    // now add the thickness of each superlayer between
    // the one requested and the first one
    for (int slyr=0; slyr<_idx; slyr++)
    {
        // add the total thickness of each intervening superlayer
        // and the midgap (distance between superlayers)
        dist2tgt += _region->superlayer(slyr).thickness()
                 +  _region->midgap();
    }

    return dist2tgt;
}

/**
 * \brief distance from target to first wire midpoint
 *
 * This is the distance from the nominal CLAS12 center to
 * the first (guard) wire in the first (guard) wire-plane
 * of this superlayer. Midpoints are the intersection points
 * of the wires with the sector mid-plane.
 *
 * \return distance (cm)
 **/
double Superlayer::first_wire_mid_dist2tgt() const
{
    return this->dist2tgt() / cos(_region->thtilt() - _thmin);
}

/**
 * \brief radial distance to first wire midpoint
 *
 * This is the radial distance from the beam-line in cm to the
 * first wire in the first (guard) wire-plane in this superlayer.
 * Midpoints are the intersection points of the wires with the
 * sector mid-plane.
 *
 * \return distance (cm)
 **/
double Superlayer::first_wire_mid_x() const
{
    return this->first_wire_mid_dist2tgt() * sin(_thmin);
}

/**
 * \brief longitudinal distance to first wire midpoint
 *
 * This is the longitudinal distance (along the beam-line from
 * the nominal CLAS12 center in cm to the first wire in the first
 * (guard) wire-plane in this superlayer. Midpoints are
 * the intersection points of the wires with the
 * sector mid-plane.
 *
 * \return distance (cm)
 **/
double Superlayer::first_wire_mid_z() const
{
    return this->first_wire_mid_dist2tgt() * cos(_thmin);
}

/**
 * \brief sense wire midpoint spacing distance
 *
 * This is the distance between midpoints of the
 * sense wires in this superlayer. Midpoints are
 * the intersection points of the wires with the
 * sector mid-plane.
 *
 * \return distance (cm)
 **/
double Superlayer::wire_mid_spacing() const
{
    // dw is the characteristic distance between sense wires
    double dw = _wpdist * 4. * cos(cons::pi<double>()/6.);
    return dw / cos(_thster);
}

/**
 * \brief wire direction cosine x
 *
 * This is \f$\cos(\alpha)\f$ where \f$\alpha\f$ is the first of the
 * usual (z-x-z) Euler angles \f$(\alpha,\beta,\gamma)\f$
 *
 * \return \f$\cos(\alpha)\f$
 **/
double Superlayer::wire_dircos_x() const
{
    return - sin(_thster) * cos(_region->thtilt());
}

/**
 * \brief wire direction cosine y
 *
 * This is \f$\cos(\beta)\f$ where \f$\beta\f$ is the second of the
 * usual (z-x-z) Euler angles \f$(\alpha,\beta,\gamma)\f$
 *
 * \return \f$\cos(\beta)\f$
 **/
double Superlayer::wire_dircos_y() const
{
    return cos(_thster);
}

/**
 * \brief wire direction cosine z
 *
 * This is \f$\cos(\gamma)\f$ where \f$\gamma\f$ is the third of the
 * usual (z-x-z) Euler angles \f$(\alpha,\beta,\gamma)\f$
 *
 * \return \f$\cos(\gamma)\f$
 **/
double Superlayer::wire_dircos_z() const
{
    return sin(_thster) * sin(_region->thtilt());
}

/**
 * \brief 3D direction of a wire in this superlayer
 *
 * Note that the direction of all wires in this superlayer is the same.
 *
 * \return direction (unit) vector in the direction of the wires
 **/
direction_vector<double,3> Superlayer::wire_direction(coordsys_t coordsys) const
{
    direction_vector<double,3> d {
        this->wire_dircos_x(),
        this->wire_dircos_y(),
        this->wire_dircos_z() };

    switch (coordsys)
    {
        case COORDSYS::SECTOR:
            break;
        case COORDSYS::CLAS:
            d = _region->sector().sector_to_clas(d);
            break;
        default:
            throw runtime_error("can not calculate wire direction in "+coord2str(coordsys)+" coordinates");
            break;
    }

    return d;
}

} /* namespace clas12::geometry::drift_chamber */
} /* namespace clas12::geometry */
} /* namespace clas12 */
