#ifndef CLAS12_GEOMETRY_PRESHOWER_CAL_PANEL_HPP
#define CLAS12_GEOMETRY_PRESHOWER_CAL_PANEL_HPP

#include <iostream>
using std::clog;
using std::endl;

#include <cmath>
#include <cstddef>
#include <string>
#include <memory>
#include <vector>

#include "geometry/plane.hpp"
#include "geometry/euclid_vector.hpp"
#include "geometry/direction_vector.hpp"

#include "clas12/geometry/coordsys.hpp"

namespace clas12
{
namespace geometry
{

class PreshowerCal;

namespace preshower_cal
{

using std::sin;
using std::cos;
using std::unique_ptr;
using std::string;
using std::vector;

using ::geometry::euclid_vector;
using ::geometry::direction_vector;
using ::geometry::plane;

class Sector;

/** \class View
 * \brief A view of paddles in a sector of the forward time of flight
 *
 * There are three such views in each sector of CLAS12
 **/
class View
{
  public:
    // inline methods
    const Sector& sector() const;

    const vector<bool>& paddles() const;
    bool paddle(const int& p) const;

    size_t npaddles() const;
    const double& dist2edge() const;
    const double& thtilt() const;
    const double& thmin() const;
    const double& paddle_width() const;
    const double& paddle_gap() const;
    const double& paddle_pairgap() const;
    const double& paddle_thickness() const;
    const double& wrapper_thickness() const;

    // methods in cpp file
    string name() const;

    double paddle_center_x(const int& p    ) const;
    double paddle_center_y(const int& p = 0) const;
    double paddle_center_z(const int& p    ) const;
    euclid_vector<double,3> paddle_center(const int& p, coordsys_t coordsys=COORDSYS::SECTOR) const;

    vector<double> paddle_centers_x() const;
    vector<double> paddle_centers_y() const;
    vector<double> paddle_centers_z() const;
    vector<euclid_vector<double,3>> paddle_centers(coordsys_t coordsys=COORDSYS::SECTOR) const;

    double paddle_length(const int& p) const;
    vector<double> paddle_lengths() const;

    direction_vector<double,3> view_normal(coordsys_t coordsys=COORDSYS::SECTOR) const;
    plane<double> view_plane(coordsys_t coordsys=COORDSYS::SECTOR) const;
    euclid_vector<double,3> center(coordsys_t coordsys=COORDSYS::SECTOR) const;
    double dist2tgt() const;
    double radial_extent() const;

    direction_vector<double,3> paddle_direction(coordsys_t coordsys=COORDSYS::SECTOR) const;
    euclid_vector<double,3> paddle_extent(coordsys_t coordsys=COORDSYS::SECTOR) const;

  private:
    View(const Sector* sector, const size_t& idx);
    View(const View& that, const Sector* sector, const size_t& idx);

    /// \brief a pointer to the parent sector
    const Sector* _sector;

    /// \brief the index of the view in the vector<View>
    /// object held by the Sector parent class
    size_t _idx;

    /// \brief the paddles in this view are either on or off
    vector<bool> _paddles;

    ///\brief number of paddles in this view
    size_t _npaddles;

    /// \brief distance from the target to the small angle
    /// upstream edge of the wrapper around this view's
    /// first counter. (cm)
    double _dist2edge;

    /// \brief tilt angle (relative to z) of the view
    double _thtilt;

    /// \brief polar angle to the first paddle's lowest edge (deg)
    double _thmin;

    /// \brief paddle's dimension perpendicular to the normal of the
    /// view's plane and in the mid-plane of the sector (cm)
    double _paddle_width;

    /// \brief gap between adjacent paddles in Views 1a and 2 or pairs of paddles for View1b.
    double _paddle_gap;

    /// \brief
    double _paddle_pairgap;

    /// \brief paddle's dimension along the normal to the
    /// view's plane (cm)
    double _paddle_thickness;

    /// \brief thickness of wrapping materials (cm)
    ///
    double _wrapper_thickness;

    /// \brief lengths of paddles perpendicular to the
    /// sector's mid-plane (measured)
    vector<double> _paddle_meas_lengths;

     /// \brief slopes of a linear fit (paddle length vs paddle number)
    vector<double> _paddle_slopes;

    /// \brief intercepts of a linear fit (paddle length vs paddle number)
    vector<double> _paddle_interXs;

    // private inline methods
    size_t paddle_index(const int& idx) const;

    // private methods in cpp file
    const vector<double>& paddle_meas_lengths() const;
    const vector<double>& paddle_slopes() const;
    const vector<double>& paddle_interXs() const;

    const double& paddle_meas_length(const int& p) const;
    const double& paddle_slope(const int& p) const;
    const double& paddle_interX(const int& p) const;

    /// \brief deleted copy constructor
    View(const View&) = delete;

    /// \brief deleted assignment operator
    View& operator=(const View&) = delete;

    /// \brief the top-level PCal class has control of all
    /// private members of this class.
    friend class ::clas12::geometry::PreshowerCal;

    /// \brief the "parent" class Sector takes care of
    /// copying this class when the top-level PCal class's
    /// copy constructor is called.
    friend class Sector;
};

/**
 * \brief get the parent sector object
 * \return constant reference to View::_sector
 **/
inline
const Sector& View::sector() const
{
    return *_sector;
}

/**
 * \brief Get a vector of paddles in this view
 * \return const reference to View::_paddles
 **/
inline
const vector<bool>& View::paddles() const
{
    return _paddles;
}

/**
 * \brief Get the status of (on/off) of a paddle in this view
 * \param [in] p a paddle in this view (counting from zero)
 * \return copy of View::_paddle[p]
 **/
inline
bool View::paddle(const int& p) const
{
    return _paddles[paddle_index(p)];
}

/**
 * \brief Get the number of paddles in this view
 * \return copy of View::paddles.size()
 **/
inline
size_t View::npaddles() const
{
    return _paddles.size();
}



/**
 * \brief Get the dist2edge
 * \return reference to View::_dist2edge
 **/
inline
const double& View::dist2edge() const
{
    return _dist2edge;
}

/**
 * \brief Get the tilt angle (relative to z) of this view
 *
 * This is the angle between the z-axis and the normal to
 * the paddles.
 *
 * \return const reference to View::_thtilt (radians)
 **/
inline
const double& View::thtilt() const
{
    return _thtilt;
}

/**
 * \brief Get the  polar angle of the ray marking dist2edge (deg)
 *
 *
 * \return const reference to View::_thmin (radians)
 **/



inline
const double& View::thmin() const
{
    return _thmin;
}

/**
 * \brief Get the paddle dimension along X in this view
 * \return const reference to View::_paddlewidth
 **/
inline
const double& View::paddle_width() const
{
    return _paddle_width;
}

inline
const double& View::paddle_gap() const
{
    return _paddle_gap;
}

inline
const double& View::paddle_pairgap() const
{
    return _paddle_pairgap;
}

/**
 * \brief Get the paddle dimension along Z in this view
 * \return const reference to View::_paddle_thickness
 **/
inline
const double& View::paddle_thickness() const
{
    return _paddle_thickness;
}

/**
 * \brief Get the thickness of wrapping materials
 * \return const reference to View::_wrapper_thickness
 **/
inline
const double& View::wrapper_thickness() const
{
    return _wrapper_thickness;
}


/**
 * \brief Measured lengths of paddles in this view
 * \return paddle lengths  (cm)
 **/
inline
const vector<double>& View::paddle_meas_lengths() const
{
    return _paddle_meas_lengths;
}

inline

/**
 * \brief Measured length of a paddle in this view
 * \return paddle length  (cm)
 **/
const double& View::paddle_meas_length(const int& p) const
{
    return _paddle_meas_lengths.at(this->paddle_index(p));

}

inline
/**
 * \brief Linear fit slope for a paddle in this view
 * \return paddle length  (cm)
 **/
const double& View::paddle_slope(const int& p) const
{
    return _paddle_slopes.at(this->paddle_index(p));

}


inline
/**
 * \brief Linear fit intercept for a paddle in this view
 * \return paddle length  (cm)
 **/
const double& View::paddle_interX(const int& p) const
{
    return _paddle_interXs.at(this->paddle_index(p));

}



/**
 * \brief convert negative indexes to positive counting from end
 * \param [in] p is index either from zero or from -1 (counting from end)
 * \return unsigned int index of the paddle in this view
 **/
inline
size_t View::paddle_index(const int& p) const
{
    return p<0 ? (_paddles.size()+p) : p;
}

} // namespace clas12::geometry::preshower_cal
} // namespace clas12::geometry
} // namespace clas12

#endif // CLAS12_GEOMETRY_PRESHOWER_CAL_PANEL_HPP
