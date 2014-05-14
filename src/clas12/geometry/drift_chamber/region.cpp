#include <cstddef>
#include <sstream>
#include <stdexcept>

#include "geometry/euclid_vector.hpp"
#include "geometry/direction_vector.hpp"
#include "geometry/plane.hpp"

#include "clas12/geometry/drift_chamber.hpp"

namespace clas12
{
namespace geometry
{
namespace drift_chamber
{

using std::stringstream;
using std::logic_error;

using ::geometry::euclid_vector;
using ::geometry::direction_vector;
using ::geometry::plane;

/**
 * \brief constructor.
 *
 * This forces the region to have a parent sector
 * object pointer.
 *
 * \param [in] sector a pointer to the parent Sector class
 * \param [in] idx the index of this region in vector<Region>
 *             held by parent Sector.
 **/
Region::Region(const Sector* sector, size_t idx)
: _sector(sector)
, _idx(idx)
{
}

/**
 * \brief copy constructor.
 *
 * This forces the region to have a parent sector
 * object pointer.
 *
 * \param [in] that the Region being copied
 * \param [in] sector a pointer to the parent Sector class
 * \param [in] idx the index of this region in vector<Region>
 *             held by parent Sector.
 **/
Region::Region(const Region& that, const Sector* sector, size_t idx)
: _sector(sector)
, _idx(idx)
, _dist2tgt(that._dist2tgt)
, _frontgap(that._frontgap)
, _midgap(that._midgap)
, _backgap(that._backgap)
, _thopen(that._thopen)
, _thtilt(that._thtilt)
, _xdist(that._xdist)
{
    for (size_t i=0; i<that._superlayers.size(); i++)
    {
        const Superlayer& superlayer = *that._superlayers[i];
        _superlayers.emplace_back(new Superlayer(superlayer,this,i));
    }
}


/**
 * \brief Get the thickness of this region
 *
 * This is sum of the frontgap, midgap, backgap and the thicknesses
 * of each superlayer in this region.
 *
 * \return thickness of this region
 **/
double Region::thickness() const
{
    // sum up the gaps
    double thickness = _frontgap + _midgap + _backgap;

    // add each superlayer's thickness
    for (auto slyr = _superlayers.begin(); slyr!=_superlayers.end(); ++slyr)
    {
        thickness += (*slyr)->thickness();
    }

    return thickness;
}

/**
 * \brief The left end-plane plane
 * \return plane(point on plane, normal) in sector coordinate system (cm)
 **/
plane<double> Region::left_end_plate(coordsys_t coordsys) const
{
    // first, calculate the plane in sector coords
    plane<double> ret{
        // point on plane
        euclid_vector<double,3>{
            _xdist,
            0.,
            0.
        },
        // normal to plane
        direction_vector<double,3>{
            sin(0.5*_thopen),
            cos(0.5*_thopen),
            0.
        }
    };

    switch (coordsys)
    {
        case COORDSYS::SECTOR:
        {
            // do nothing
            break;
        }
        case COORDSYS::CLAS:
        {
            // rotate to clas coordinates
            ret = _sector->sector_to_clas(ret);
            break;
        }
        default:
        {
            stringstream err;
            err << "can not transform endplate to coordinate"
                   " system other than CLAS or SECTOR.";
            throw logic_error(err.str());
        }
    }

    return ret;
}

/**
 * \brief The right end-plane plane
 * \return plane(point on plane, normal) in sector coordinate system (cm)
 **/
plane<double> Region::right_end_plate(coordsys_t coordsys) const
{
    // first, calculate the plane in sector coords
    plane<double> ret{
        // point on plane
        euclid_vector<double,3>{
            _xdist,
            0.,
            0.
        },
        // normal to plane
        direction_vector<double,3>{
             sin(0.5*_thopen),
            -cos(0.5*_thopen),
            0.
        }
    };

    switch (coordsys)
    {
        case COORDSYS::SECTOR:
        {
            // do nothing
            break;
        }
        case COORDSYS::CLAS:
        {
            // rotate to clas coordinates
            ret = _sector->sector_to_clas(ret);
            break;
        }
        default:
        {
            stringstream err;
            err << "can not transform endplate to coordinate"
                   " system other than CLAS or SECTOR.";
            throw logic_error(err.str());
        }
    }

    return ret;
}

/**
 * \brief The center-point of this region
 *
 * The x and z coordinates are taken as the midpoint between
 * these two point:
 * p0 = endpoint of the first guard wire in the first (gaurd)
 *      wire-plane in the first superlayer of this region
 * p1 = endpoint of the last guard wire in the last (guard)
 *      wire-plane in the last superlayer of this region
 * The y-coordinate is always zero since this is in the sector
 * coordinate system.
 *
 * \return (x,y,z) position in sector-coordinates of this region (cm)
 **/
euclid_vector<double,3> Region::center(coordsys_t coordsys) const
{
    // first, calculate the point in sector coords
    euclid_vector<double,3> p0 = \
        this->superlayer( 0).guardlayer( 0).wire( 0).end_point();
    euclid_vector<double,3> p1 = \
        this->superlayer(-1).guardlayer(-1).wire(-1).end_point();

    euclid_vector<double,3> region_center = 0.5 * (p0 + p1);
    region_center.y(0);

    switch (coordsys)
    {
        case COORDSYS::SECTOR:
        {
            // do nothing
            break;
        }
        case COORDSYS::CLAS:
        {
            // rotate to clas coordinates
            region_center = _sector->sector_to_clas(region_center);
            break;
        }
        default:
        {
            stringstream err;
            err << "can not transform region center to coordinate"
                   " system other than CLAS or SECTOR.";
            throw logic_error(err.str());
        }
    }

    return region_center;
}

} /* namespace clas12::geometry::drift_chamber */
} /* namespace clas12::geometry */
} /* namespace clas12 */
