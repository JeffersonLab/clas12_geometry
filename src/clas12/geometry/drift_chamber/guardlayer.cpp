#include <cstddef>
#include <cmath>
#include <stdexcept>

#include "geometry/euclid_vector.hpp"
#include "geometry/line.hpp"
#include "geometry/line_segment.hpp"
#include "geometry/plane.hpp"

#include "clas12/geometry/drift_chamber.hpp"

namespace clas12
{
namespace geometry
{
namespace drift_chamber
{

using std::runtime_error;

using std::cos;

using ::geometry::euclid_vector;
using ::geometry::line;
using ::geometry::line_segment;
using ::geometry::plane;

/**
 * \brief constructor.
 *
 * This forces the guard layer to have a parent superlayer
 * object pointer.
 *
 * \param [in] superlayer a pointer to the parent Superlayer class
 * \param [in] idx the index of this guardlayer in vector<Guardlayer>
 *             held by parent Superlayer.
 **/
Guardlayer::Guardlayer(const Superlayer* superlayer, size_t idx)
: _superlayer(superlayer)
, _idx(idx)
{
}

/**
 * \brief copy constructor.
 *
 * This forces the guard layer to have a parent superlayer
 * object pointer.
 *
 * \param [in] that the Guardlayer being copied
 * \param [in] superlayer a pointer to the parent Superlayer class
 * \param [in] idx the index of this guardlayer in vector<Guardlayer>
 *             held by parent Superlayer.
 **/
Guardlayer::Guardlayer(const Guardlayer& that, const Superlayer* superlayer, size_t idx)
: _superlayer(superlayer)
, _idx(idx)
, _nwires(that._nwires)
{
}


/**
 * \brief x-position of the midpoint of a wire in this layer
 * \param [in] w the wire index (starting from zero) in this layer
 * \return x position in sector coordinate system (cm)
 **/
double Guardlayer::wire_mid_x(int w) const
{
    double xmid = _superlayer->first_wire_mid_x();

    if (_idx == 1)
    {
        xmid += _superlayer->thickness() * sin(_superlayer->region().thtilt());
    }

    double wmidsp = _superlayer->wire_mid_spacing();

    // stagger even wire planes
    double nwire = double(this->wire_index(w)) + ((_idx % 2) ? -0.5 : 0.);
    xmid += nwire * wmidsp * cos(_superlayer->region().thtilt());

    return xmid;
}

/**
 * \brief y-position of the midpoint of a wire in this layer
 * \param [in] w the wire index (starting from zero) in this layer
 * \return y position in sector coordinate system (cm)
 **/
double Guardlayer::wire_mid_y(int w /*=0*/) const
{
    return 0.;
}

/**
 * \brief z-position of the midpoint of a wire in this layer
 * \param [in] w the wire index (starting from zero) in this layer
 * \return z position in sector coordinate system (cm)
 **/
double Guardlayer::wire_mid_z(int w) const
{
    double zmid = _superlayer->first_wire_mid_z();

    if (_idx == 1)
    {
        zmid += _superlayer->thickness() * cos(_superlayer->region().thtilt());
    }

    double wmidsp = _superlayer->wire_mid_spacing();

    // stagger even wire planes
    double nwire = double(this->wire_index(w)) + ((_idx % 2) ? -0.5 : 0.);
    zmid -= nwire * wmidsp * sin(_superlayer->region().thtilt());

    return zmid;
}

/**
 * \brief position of the midpoint of a wire in this layer
 * \param [in] w the wire index (starting from zero) in this layer
 * \return (x,y,z) position in sector coordinate system (cm)
 **/
euclid_vector<double,3> Guardlayer::wire_mid(int w, coordsys_t coordsys) const
{
    euclid_vector<double,3> mid {
        this->wire_mid_x(w),
        this->wire_mid_y(w),
        this->wire_mid_z(w)
    };

    switch (coordsys)
    {
        case COORDSYS::SECTOR:
            break;
        case COORDSYS::CLAS:
            mid = _superlayer->region().sector().sector_to_clas(mid);
            break;
        default:
            throw runtime_error("can not calculate wire midpoint in "+coord2str(coordsys)+" coordinates");
            break;
    }

    return mid;
}

/**
 * \brief position of the center point of a wire in this layer
 *
 * This is the wire's center point. Not to be confused with
 * the "midpoint" which is the intersection point of this
 * wire with the sector's mid-plane.
 *
 * \param [in] w the wire index (starting from zero) in this layer
 * \return (x,y,z) position in sector coordinate system (cm)
 **/
euclid_vector<double,3> Guardlayer::wire_center(int w, coordsys_t coordsys) const
{
    return this->wire(w, coordsys).mid_point();
}

/**
 * \brief distance from the origin to this wire plane
 *
 * The origin is (0,0,0) in the CLAS coordinate system
 *
 * \return distance (cm)
 *
 * \warning This assumes only one leading guardlayer and one
 * trailing guardlayer; i.e. Guardlayer::_idx is either
 * 0 or 1 and Superlayer::thickness() is the distance from one
 * guardlayer to the other.
 **/
double Guardlayer::dist2tgt() const
{
    return _superlayer->dist2tgt() + _idx * _superlayer->thickness();
}

/**
 * \brief 3D line segment representing a wire
 * \param [in] w the wire index (starting from zero) in this layer
 * \return line segment with end-points in CLAS coordinate system (cm)
 **/
line_segment<double,3> Guardlayer::wire(int w, coordsys_t coordsys) const
{
    // end plates of this region
    plane<double> lplate = _superlayer->region().left_end_plate(coordsys);
    plane<double> rplate = _superlayer->region().right_end_plate(coordsys);

    // wire as an infinitely-extending line
    line<double,3> wire_line{
        this->wire_mid(w, coordsys),
        _superlayer->wire_direction(coordsys)
    };

    // get the intersection and create line segment from one
    // point to the other.
    return line_segment<double,3>{
        intersection(wire_line, lplate),
        intersection(wire_line, rplate)
    };
}

/**
 * \brief all 3D line segments representing guard and sense wires in this layer
 * \return vector of line segments in sector coordinate system (cm)
 **/
vector<line_segment<double,3>> Guardlayer::wires(coordsys_t coordsys) const
{
    vector<line_segment<double,3>> ret;

    // end plates of this region
    plane<double> lplate = _superlayer->region().left_end_plate(coordsys);
    plane<double> rplate = _superlayer->region().right_end_plate(coordsys);

    for (size_t idx=0; idx<this->nwires(); idx++)
    {
        // wire as an infinitely-extending line
        line<double,3> wire_line{
            this->wire_mid(idx, coordsys),
            _superlayer->wire_direction(coordsys)
        };

        // get the intersection and create line segment from one
        // point to the other.
        ret.push_back(line_segment<double,3>{
            intersection(wire_line, lplate),
            intersection(wire_line, rplate)
        });
    }

    return ret;
}

/**
 * \brief length of a given guard wire
 * \param [in] w the wire index (starting from zero) in this layer
 * \return length (cm)
 **/
double Guardlayer::wire_length(int w) const
{
    return this->wire(w).length();
}

} /* namespace clas12::geometry::drift_chamber */
} /* namespace clas12::geometry */
} /* namespace clas12 */
