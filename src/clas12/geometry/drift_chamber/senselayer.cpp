#include <cstddef>
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

using ::geometry::euclid_vector;
using ::geometry::line;
using ::geometry::line_segment;
using ::geometry::plane;

/**
 * \brief constructor.
 *
 * This forces the sense layer to have a parent superlayer
 * object pointer.
 *
 * \param [in] superlayer a pointer to the parent Superlayer class
 * \param [in] idx the index of this senselayer in vector<Senselayer>
 *             held by parent Superlayer.
 **/
Senselayer::Senselayer(const Superlayer* superlayer, size_t idx)
: _superlayer(superlayer)
, _idx(idx)
{
}

/**
 * \brief copy constructor.
 *
 * This forces the sense layer to have a parent superlayer
 * object pointer.
 *
 * \param [in] that the Senselayer being copied
 * \param [in] superlayer a pointer to the parent Superlayer class
 * \param [in] idx the index of this senselayer in vector<Senselayer>
 *             held by parent Superlayer.
 **/
Senselayer::Senselayer(const Senselayer& that, const Superlayer* superlayer, size_t idx)
: _superlayer(superlayer)
, _idx(idx)
, _sensewires(that._sensewires)
, _nguardwires(that._nguardwires)
{
}

/**
 * \brief x-position of the midpoint of a wire in this layer
 *
 * Midpoints are the intersection points of the wires with
 * the sector mid-plane.
 *
 * \param [in] w the wire index (starting from zero) in this layer
 * \return x position in sector coordinate system (cm)
 **/
double Senselayer::wire_mid_x(int w) const
{
    static const size_t first_guardlayer = 1;

    // r is the distance from the guard layer to this wire plane in cm.
    double r = double(_idx + first_guardlayer)
             * _superlayer->cellthickness()
             * _superlayer->wpdist();

    double xmid = _superlayer->first_wire_mid_x()
                + r * sin(_superlayer->region().thtilt());

    double wmidsp = _superlayer->wire_mid_spacing();

    // stagger even wire planes
    double nwire = double(this->wire_index(w)) + ((_idx % 2) ? 0. : -0.5);
    xmid += nwire * wmidsp * cos(_superlayer->region().thtilt());

    return xmid;
}

/**
 * \brief y-position of the midpoint of a wire in this layer
 *
 * The y-position of the midpoint of all wires in the
 * sector coordinate system is zero. Midpoints are the
 * intersection points of the wires with the sector
 * mid-plane.
 *
 * \param [in] w wire index from zero (ignored)
 * \return y position in sector coordinate system (cm)
 **/
inline
double Senselayer::wire_mid_y(int w /*=0*/) const
{
    return 0.;
}

/**
 * \brief z-position of the midpoint of a wire in this layer
 *
 * Midpoints are the intersection points of the wires with
 * the sector mid-plane.
 *
 * \param [in] w the wire index (starting from zero) in this layer
 * \return z position in sector coordinate system (cm)
 **/
double Senselayer::wire_mid_z(int w) const
{
    static const size_t first_guardlayer = 1;

    // r is the distance from the guard layer to this wire plane in cm.
    double r = double(_idx + first_guardlayer)
             * _superlayer->cellthickness()
             * _superlayer->wpdist();

    double zmid = _superlayer->first_wire_mid_z()
                + r * cos(_superlayer->region().thtilt());

    double wmidsp = _superlayer->wire_mid_spacing();

    // stagger even wire planes
    double nwire = double(this->wire_index(w)) + ((_idx % 2) ? 0. : -0.5);
    zmid -= nwire * wmidsp * sin(_superlayer->region().thtilt());
    return zmid;
}

/**
 * \brief position of the midpoint of a wire in this layer
 *
 * Midpoints are the intersection points of the wires with
 * the sector mid-plane.
 *
 * \param [in] w the wire index (starting from zero) in this layer
 * \return (x,y,z) position in sector coordinate system (cm)
 **/
euclid_vector<double,3> Senselayer::wire_mid(int w, coordsys_t coordsys) const
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
 * \brief x-positions of the midpoints of all wires in this layer
 *
 * Midpoints are the intersection points of the wires with
 * the sector mid-plane.
 *
 * \return x positions in sector coordinate system (cm)
 **/
vector<double> Senselayer::wires_mid_x() const
{
    vector<double> ret(this->nwires());
    for (size_t w=0; w<this->nwires(); w++)
    {
        ret[w] = this->wire_mid_x(w);
    }
}

/**
 * \brief y-positions of the midpoints of all wires in this layer
 *
 * Midpoints are the intersection points of the wires with
 * the sector mid-plane.
 *
 * \return y positions in sector coordinate system (cm)
 **/
vector<double> Senselayer::wires_mid_y() const
{
    return vector<double>(this->nwires(),this->wire_mid_y());
}

/**
 * \brief z-positions of the midpoints of all wires in this layer
 *
 * Midpoints are the intersection points of the wires with
 * the sector mid-plane.
 *
 * \return z positions in sector coordinate system (cm)
 **/
vector<double> Senselayer::wires_mid_z() const
{
    vector<double> ret(this->nwires());
    for (size_t w=0; w<this->nwires(); w++)
    {
        ret[w] = this->wire_mid_z(w);
    }
}

/**
 * \brief vector of midpoints of all wires in this layer
 *
 * Midpoints are the intersection points of the wires with
 * the sector mid-plane.
 *
 * \return (x,y,z) positions in sector coordinate system (cm)
 **/
vector< euclid_vector<double,3> > Senselayer::wires_mid(coordsys_t coordsys) const
{
    vector< euclid_vector<double,3> > ret(this->nwires());
    for (size_t w=0; w<this->nwires(); w++)
    {
        ret[w] = this->wire_mid(w);
    }

    switch (coordsys)
    {
        case COORDSYS::SECTOR:
            break;
        case COORDSYS::CLAS:
            for (auto& x : ret)
            {
                x = _superlayer->region().sector().sector_to_clas(x);
            }
            break;
        default:
            throw runtime_error("can not calculate wire midpoints in "+coord2str(coordsys)+" coordinates");
            break;
    }

    return ret;
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
euclid_vector<double,3> Senselayer::wire_center(int w, coordsys_t coordsys) const
{
    return this->wire(w, coordsys).mid_point();
}

/**
 * \brief the center-point of this sense-layer volume
 * \return (x,y,z) position in sector coordinate system (cm)
 **/
euclid_vector<double,3> Senselayer::center(coordsys_t coordsys) const
{
    return 0.5 * (this->wire_center(0, coordsys) + this->wire_center(-1, coordsys));
}

/**
 * \brief distance from the origin to this wire plane
 *
 * The origin is (0,0,0) in the CLAS coordinate system
 *
 * \return distance (cm)
 **/
double Senselayer::dist2tgt() const
{
    return _superlayer->dist2tgt() + _idx * _superlayer->layer_thickness();
}

/**
 * \brief all 3D line segments representing guard and sense wires in this layer
 * \return vector of line segments in sector coordinate system (cm)
 **/
vector<line_segment<double,3>> Senselayer::wires(coordsys_t coordsys) const
{
    vector<line_segment<double,3>> ret;

    // end plates of this region
    plane<double> lplate = _superlayer->region().left_end_plate(coordsys);
    plane<double> rplate = _superlayer->region().right_end_plate(coordsys);

    for (size_t idx=0; idx<this->nwires(); idx++)
    {
        // wire as an infinitely-extending line
        line<double,3> wire_line{
            this->wire_mid(idx,coordsys),
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
 * \brief 3D line segment representing a wire
 * \param [in] w the wire index (starting from zero) in this layer
 * \return line segment with end-points in sector coordinate system (cm)
 **/
line_segment<double,3> Senselayer::wire(int w, coordsys_t coordsys) const
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
 * \brief length of a given sense wire
 * \param [in] w the wire index (starting from zero) in this layer
 * \return length (cm)
 **/
double Senselayer::wire_length(int w) const
{
    return this->wire(w).length();
}

/**
 * \brief this sense layer's wire-plane
 * \return plane(point on plane, normal) in sector coordinate system (cm)
 **/
plane<double> Senselayer::wire_plane(coordsys_t coordsys) const
{
    return plane<double>{
        this->wire_mid(0,coordsys),
        _superlayer->wire_direction(coordsys)
    };
}

} // namespace clas12::geometry::drift_chamber
} // namespace clas12::geometry
} // namespace clas12
