#ifndef CLAS12_GEOMETRY_ELECTROMAGNETIC_CAL_SECTOR_HPP
#define CLAS12_GEOMETRY_ELECTROMAGNETIC_CAL_SECTOR_HPP

#include <iostream>
using std::clog;
using std::endl;

#include <cstddef>
#include <memory>
#include <vector>
#include <string>
#include <map>

#include "layer.hpp"

namespace clas12
{
namespace geometry
{

class ElectromagneticCal;

namespace electromagnetic_cal
{

using std::size_t;
using std::unique_ptr;
using std::vector;
using std::string;
using std::map;

using ::geometry::euclid_vector;
using ::geometry::direction_vector;
using ::geometry::line;
using ::geometry::line_segment;
using ::geometry::plane;

using ::clas12::geometry::ElectromagneticCal;

/**
 * \brief a sector of the EC which consists of 13 layers
 *
 * There are six sectors of EC in CLAS12 which hold 13 layers each
 **/
class Sector
{
  public:
    // inline methods
    const ElectromagneticCal& ec() const;

    const vector<unique_ptr<Layer>>& layers() const;
    const Layer& layer(int l) const;
    const Layer& layer(const string& l) const;

    size_t nlayers() const;
    const double& thtilt() const;
    const double& alum_thick() const;
    const double& dist2tgt() const;
    const double& dist2cnt() const;
    const double& a1() const;
    const double& a2() const;
    const double& a3() const;
    const double& d2() const;
    const double& d2prime() const;

    size_t index() const;

    euclid_vector<double,3>    sector_to_clas(const euclid_vector<double,3>& v   ) const;
    direction_vector<double,3> sector_to_clas(const direction_vector<double,3>& v) const;
    line<double,3>             sector_to_clas(const line<double,3>& l            ) const;
    line_segment<double,3>     sector_to_clas(const line_segment<double,3>& l    ) const;
    plane<double>              sector_to_clas(const plane<double>& p             ) const;


  private:
    Sector(const ElectromagneticCal* ec, size_t idx);
    Sector(const Sector& that, const ElectromagneticCal* ec, size_t idx);

    /// \brief a pointer to the parent sector
    const ElectromagneticCal* _ec;

    /// \brief the index of the sector in the vector<Sector>
    /// object held by the ElectromagneticCal parent class
    size_t _idx;

    /// \brief a sector consists of 13 layers
    vector<unique_ptr<Layer>> _layers;

    /// \brief convert negative index to run from the end of the vector
    size_t layer_index(int idx) const;


    ///\brief number of layers in this sector
    size_t _nlayers;

    /// \brief angle in degrees of the sector tilt with respect to the z - axis
    double _thtilt;

    /// \brief thickness of a steel plate (cm)
    double _alum_thick;

    /// \brief length of a normal line connecting CLAS12 target and the first scintillator face (cm)
    double _dist2tgt;

   /// \brief distance from the perpendicular point to the center of the front face of EC sector (cm)
    double _dist2cnt;

   /// \brief spacing in local y coordinates between centers of each scintillator layer (cm)
    double _a1;

   /// \brief half-height of scintillator layer 1 from near-beam vertex to midpoint of large-angle side (cm)
    double _a2;

   /// \brief increase in half-height between adjacent scintillator layers (cm)
    double _a3;

   /// \brief length of short-side of trapezoid on end of strip to connect with PMT for layer <16 (cm)
    double _d2;

   /// \brief length of short-side of trapezoid on end of strip to connect with PMT for layer >15 (cm)
    double _d2prime;


    // private inline methods

    //size_t layer_index(int idx) const;
    //size_t layer_index(const string& id) const;


    /// \brief deleted copy constructor
    Sector(const Sector&) = delete;

    /// \brief deleted assignment operator
    Sector& operator=(const Sector&) = delete;

    /// \brief the top-level EC class has control of all
    /// private members of this class.
    friend class ::clas12::geometry::ElectromagneticCal;
};

/**
 * \brief get the parent ec object
 * \return constant reference to Sector::_ec
 **/
inline const ElectromagneticCal& Sector::ec() const
{
    return *_ec;
}

/** \fn Sector::layers()
 * \brief Get a vector of the layers in this sector
 * \return const reference to Sector::_layers
 **/
inline const vector<unique_ptr<Layer>>& Sector::layers() const
{
    return _layers;
}

/**
 * \brief Get a layer in this sector
 * \param [in] l The layer index within this sector (counting from zero)
 * \return const reference to Sector::_layers[l]
 **/
inline const Layer& Sector::layer(int l) const
{
    return *_layers[this->layer_index(l)];
}



/**
 * \brief convert negative indexes to positive counting from end
 * \param [in] p is index either from zero or from -1 (counting from end)
 * \return unsigned int index of the layer in this view
 **/
inline
size_t Sector::layer_index(int p) const
{
    return p<0 ? (_layers.size()+p) : p;
}


/**
 * \brief Get a index of this sector in the parent region
 * \return const reference to Sector::_idx
 **/
inline
size_t Sector::index() const
{
    return _idx;
}


/**
 * \brief Get the number of layers in this sector
 * \return copy of Sector::_nlayers
 **/
inline
size_t Sector::nlayers() const
{
    return _nlayers;
}


/**
 * \brief Get the tilt angle
 * \return reference to Sector::_thtilt
 **/
inline
const double& Sector::thtilt() const
{
    return _thtilt;
}

/**
 * \brief Get the alum thickness
 * \return reference to Sector::_alum_thick
 **/
inline
const double& Sector::alum_thick() const
{
    return _alum_thick;
}

/**
 * \brief Get the distance to target
 * \return reference to Sector::_dist2tgt
 **/
inline
const double& Sector::dist2tgt() const
{
    return _dist2tgt;
}

/**
 * \brief Get the distance from perp. point to geometric center
 * \return reference to Sector::_dist2tgt
 **/
inline
const double& Sector::dist2cnt() const
{
    return _dist2cnt;
}

/**
 * \brief Get the spacing in local y coordinates between centers of each scintillator layer
 * \return reference to Sector::_a1
 **/
inline
const double& Sector::a1() const
{
    return _a1;
}

/**
 * \brief Get the half-height of scintillator layer 1
 * \return reference to Sector::_a2
 **/
inline
const double& Sector::a2() const
{
    return _a2;
}

/**
 * \brief Get the increase in half-height between adjacent scintillator layers
 * \return reference to Sector::_a3
 **/
inline
const double& Sector::a3() const
{
    return _a3;
}

/**
 * \brief Get the length of short-side of trapezoid on end of strip to connect with PMT for layer <16
 * \return reference to Sector::_d2
 **/
inline
const double& Sector::d2() const
{
    return _d2;
}

/**
 * \brief Get the length of short-side of trapezoid on end of strip to connect with PMT for layer >15
 * \return reference to Sector::_d2prime
 **/
inline
const double& Sector::d2prime() const
{
    return _d2prime;
}

} // namespace clas12::geometry::electromagnetic_cal
} // namespace clas12::geometry
} // namespace clas12

#endif // CLAS12_GEOMETRY_ELECTROMAGNETIC_CAL_SECTOR_HPP
