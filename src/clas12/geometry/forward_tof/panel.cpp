#include <iostream>
using std::clog;
using std::endl;

#include "geometry/io.hpp"


#include <cstddef>
#include <cstdlib>
#include <sstream>
#include <stdexcept>

#include "geometry/euclid_vector.hpp"
#include "geometry/direction_vector.hpp"
#include "geometry/plane.hpp"

#include "clas12/geometry/forward_tof.hpp"

namespace clas12
{
namespace geometry
{
namespace forward_tof
{
using std::stringstream;
using std::logic_error;
using std::div;
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
 * \param [in] idx the index of this panel in vector<Panel>
 *             held by parent Sector.
 **/
Panel::Panel(const Sector* sector, const size_t& idx)
: _sector(sector)
, _idx(idx)
{}

/**
 * \brief copy constructor.
 *
 * This forces the panel to have a parent sector
 * object pointer.
 *
 * \param [in] that the Panel being copied
 * \param [in] sector a pointer to the parent Sector class
 * \param [in] idx the index of this region in vector<Panel>
 *             held by parent Sector.
 **/
Panel::Panel(const Panel& that, const Sector* sector, const size_t& idx)
: _sector(sector)
, _idx(idx)
, _paddles(that._paddles)
, _npaddles(that._npaddles)
, _dist2edge(that._dist2edge)
, _thtilt(that._thtilt)
, _thmin(that._thmin)
, _paddle_width(that._paddle_width)
, _paddle_gap(that._paddle_gap)
, _paddle_pairgap(that._paddle_pairgap)
, _paddle_thickness(that._paddle_thickness)
, _wrapper_thickness(that._wrapper_thickness)
, _paddle_meas_lengths(that._paddle_meas_lengths)
{}

/**
 * \brief get the name of this panel (1a, 1b, or 2)
 * \return the string "1a", "1b" or "2"
 **/
string Panel::name() const
{
    return _sector->panel_name(_idx);
}

double Panel::paddle_center_x(const int& p) const
{
    size_t pidx = this->paddle_index(p);
    double costhtilt = cos(_thtilt);
    double p2pdist;
    double gaptot;

    double x = _dist2edge * sin(_thmin) + (0.5*_paddle_width+_wrapper_thickness) * costhtilt;

    if ((_idx == 0) || (_idx == 2)) // Panels 1a and 2
    {
        p2pdist = _paddle_width + _paddle_gap + 2.*_wrapper_thickness;
        x += pidx * p2pdist * costhtilt;
    }
    else // Panel 1b
    {
        p2pdist = _paddle_width + 2.*_wrapper_thickness;
        gaptot = double(div(pidx+1,2).quot) * _paddle_gap + double(div(pidx,2).quot) * _paddle_pairgap;
        x += (pidx * p2pdist + gaptot) * costhtilt;
    }

    // at this point, we have the face edge (outside of the wrapper)
    // we now move to the center of the volume
    //x += (0.5*_paddle_thickness + _wrapper_thickness) * sin(_thtilt);

    return x;
}

double Panel::paddle_center_y(const int& p) const
{
    return 0;
}

double Panel::paddle_center_z(const int& p) const
{
    size_t pidx = this->paddle_index(p);
    double p2pdist;
    double gaptot;

    double z = _dist2edge * cos(_thmin) - (0.5*_paddle_width+_wrapper_thickness) * sin(_thtilt);

    if ((_idx == 0) || (_idx == 2)) // Panels 1a and 2
    {
        p2pdist = _paddle_width + _paddle_gap + 2.*_wrapper_thickness;
        z -= pidx * p2pdist * sin(_thtilt);
    }
    else // Panel 1b
    {
        p2pdist = _paddle_width + 2.*_wrapper_thickness;
        gaptot = div(pidx+1,2).quot * _paddle_gap + div(pidx,2).quot * _paddle_pairgap;
        z -= (pidx * p2pdist + gaptot) * sin(_thtilt);
    }

    // at this point, we have the face edge (outside of the wrapper)
    // we now move to the center of the volume
    //z -= (0.5*_paddle_thickness + _wrapper_thickness) * cos(_thtilt);

    return z;
}

euclid_vector<double,3> Panel::paddle_center(const int& p, coordsys_t coordsys) const
{
    euclid_vector<double,3> center{
        this->paddle_center_x(p),
        this->paddle_center_y(p),
        this->paddle_center_z(p) };

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
            center = _sector->sector_to_clas(center);
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

    return center;


}


vector<double> Panel::paddle_centers_x() const
{
    vector<double> ret(_paddles.size());
    for (size_t p=0; p<_paddles.size(); p++)
    {
        ret[p] = this->paddle_center_x(p);
    }
    return ret;
}

vector<double> Panel::paddle_centers_y() const
{
    return vector<double>(_paddles.size(), 0);
}

vector<double> Panel::paddle_centers_z() const
{
    vector<double> ret(_paddles.size());
    for (size_t p=0; p<_paddles.size(); p++)
    {
        ret[p] = this->paddle_center_z(p);
    }
    return ret;
}

/**
 * \brief positions of paddle centers
 *
 * \return (x,y,z) position in sector coordinate system (cm)
 **/
vector<euclid_vector<double,3>> Panel::paddle_centers(coordsys_t coordsys) const
{
    vector<euclid_vector<double,3>> ret(this->paddles().size());
    for (size_t i=0; i<this->paddles().size(); i++)
    {
        ret[i] = this->paddle_center(i, coordsys);
    }
    return ret;
}

/**
 * \brief paddle length obtained with a linear fit (cm)
 *
 **/
double Panel::paddle_length(const int& p) const
{

    return (this->paddle_slope(p))*(p+1) + this->paddle_interX(p);
}

/**
 * \brief a vector of all paddle lengths in this panel, obtained with a linear fit (cm)
 *
 **/
vector<double> Panel::paddle_lengths() const
{
    vector<double> ret(_paddles.size());
    for (size_t p=0; p<_paddles.size(); p++)
    {
        ret[p] = this->paddle_length(p);
    }
    return ret;
}



direction_vector<double,3> Panel::panel_normal(coordsys_t coordsys) const
{
    direction_vector<double,3> n = direction_vector<double,3>{0, _thtilt};
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
            n = _sector->sector_to_clas(n);
            break;
        }
        default:
        {
            stringstream err;
            err << "can not transform panel normal to coordinate"
                   " system other than CLAS or SECTOR.";
            throw logic_error(err.str());
        }
    }
    return n;
}

plane<double> Panel::panel_plane(coordsys_t coordsys) const
{
    plane<double> p = plane<double>{
        this->paddle_center(0),
        this->panel_normal() };
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
            p = _sector->sector_to_clas(p);
            break;
        }
        default:
        {
            stringstream err;
            err << "can not transform panel normal to coordinate"
                   " system other than CLAS or SECTOR.";
            throw logic_error(err.str());
        }
    }
    return p;
}

/**
 * distance from the target to the plane defined by the centers of the paddles along the normal to the plane
 *
 **/
double Panel::dist2tgt() const
{
     return _dist2edge*(cos(_thtilt - _thmin));
}


/**
 * \brief Overall radial extent of this panel:
 * the length along the sector midplane from the inside edge of counter #1
 * to the outside edge of the last counter
 **/
double Panel::radial_extent() const
{
     int np =  this->npaddles();
     int nnp = np -1;
     double ret = np*_paddle_width + 2*_wrapper_thickness*np +(np-1)*_paddle_gap;
     if (np == 62) ret = np*_paddle_width + 2*_wrapper_thickness*np +(np/2.-1)*_paddle_gap;
     return ret;

     //euclid_vector<double,3> deltaz = this->paddle_center(nnp) - this->paddle_center(0);
     //double posz = deltaz.r();
     //return posz;
}



/**
 * \brief The center-point of this panel
 * x is found as a midpoint between the first and the last paddle center-x positions
 * The y-coordinate is always zero since this is in the sector
 * coordinate system.
 * z is found as a midpoint between the first and the last paddle center-z positions
 * \return (x,y,z) position in sector-coordinates of this region (cm)
 **/
euclid_vector<double,3> Panel::center(coordsys_t coordsys) const
{
    euclid_vector<double,3> first = this->paddle_center(0, coordsys);
    euclid_vector<double,3> last  = this->paddle_center(-1, coordsys);

    clog << "first paddle: " << first << endl;
    clog << "last paddle:  " << last << endl;


    return 0.5 * (first + last);
}

/**
 *
 **/
direction_vector<double,3> Panel::paddle_direction(coordsys_t coordsys) const
{
    direction_vector<double,3> ret{0,1,0}; // (x,y,z) = (0,1,0)
    switch (coordsys)
    {
        case COORDSYS::SECTOR:
        {
            return ret;
        }
        case COORDSYS::CLAS:
        {
            // rotate to clas coordinates
            return _sector->sector_to_clas(ret);
        }
        default:
        {
            stringstream err;
            err << "can not transform region center to coordinate"
                   " system other than CLAS or SECTOR.";
            throw logic_error(err.str());
        }
    }

}

euclid_vector<double,3> Panel::paddle_extent(coordsys_t coordsys) const
{
    switch (coordsys)
    {
        case COORDSYS::LAYER:
        {
            // LAYER coordinates
            return euclid_vector<double,3>{_paddle_width, 0., _paddle_thickness};
        }
        case COORDSYS::SECTOR:
        {
            // rotate to sector coordinates
            const double& w = _paddle_width;
            const double& t = _paddle_thickness;
            double ca = cos(_thtilt);
            double sa = sin(_thtilt);
            return euclid_vector<double,3>{w*ca+t*sa, 0, w*sa+t*ca};
        }
        default:
        {
            stringstream err;
            err << "can not transform paddle extent to coordinate"
                   " system other than SECTOR or LAYER.";
            throw logic_error(err.str());
        }
    }
}

} // namespace clas12::geometry::forward_tof
} // namespace clas12::geometry
} // namespace clas12
