#ifndef CLAS12_GEOMETRY_PRESHOWER_CAL_SECTOR_HPP
#define CLAS12_GEOMETRY_PRESHOWER_CAL_SECTOR_HPP

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

class PreshowerCal;

namespace preshower_cal
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

using ::clas12::geometry::PreshowerCal;

/**
 * \brief a sector of the PCal which consists of 5 layers
 *
 * There are six sectors of PCal in CLAS12 which hold
 * 5 layers each
 **/
class Sector
{
  public:
    // inline methods
    const PreshowerCal& pcal() const;

    const vector<unique_ptr<Layer>>& layers() const;
    const Layer& layer(int l) const;
    const Layer& layer(const string& l) const;

    size_t nlayers() const;
    size_t nfoam() const;
    size_t nsteel() const;
    const double& thtilt() const;
    const double& foam_thick() const;
    const double& steel_thick() const;

    size_t index() const;

    euclid_vector<double,3>    sector_to_clas(const euclid_vector<double,3>& v   ) const;
    direction_vector<double,3> sector_to_clas(const direction_vector<double,3>& v) const;
    line<double,3>             sector_to_clas(const line<double,3>& l            ) const;
    line_segment<double,3>     sector_to_clas(const line_segment<double,3>& l    ) const;
    plane<double>              sector_to_clas(const plane<double>& p             ) const;


  private:
    Sector(const PreshowerCal* pcal, size_t idx);
    Sector(const Sector& that, const PreshowerCal* pcal, size_t idx);

    /// \brief a pointer to the parent sector
    const PreshowerCal* _pcal;

    /// \brief the index of the sector in the vector<Sector>
    /// object held by the PreshowerCal parent class
    size_t _idx;

    /// \brief a sector consists of 5 layers
    vector<unique_ptr<Layer>> _layers;

    /// \brief convert negative index to run from the end of the vector
    size_t layer_index(int idx) const;


    ///\brief number of layers in this sector
    size_t _nlayers;

    ///\brief number of foam layers in this sector
    size_t _nfoam;

    ///\brief number of steel layers in this sector
    size_t _nsteel;

    /// \brief angle in degrees of the sector tilt with respect to the z - axis
    double _thtilt;

    /// \brief thickness of a foam layer (mm)
    double _foam_thick;

    /// \brief thickness of a steel plate (mm)
    double _steel_thick;

    // private inline methods

    //size_t layer_index(int idx) const;
    //size_t layer_index(const string& id) const;


    /// \brief deleted copy constructor
    Sector(const Sector&) = delete;

    /// \brief deleted assignment operator
    Sector& operator=(const Sector&) = delete;

    /// \brief the top-level PCal class has control of all
    /// private members of this class.
    friend class ::clas12::geometry::PreshowerCal;
};

/**
 * \brief get the parent pcal object
 * \return constant reference to Sector::_pcal
 **/
inline const PreshowerCal& Sector::pcal() const
{
    return *_pcal;
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
 * \brief Get the number of foam layers in this sector
 * \return copy of Sector::_nfoam
 **/
inline
size_t Sector::nfoam() const
{
    return _nfoam;
}

/**
 * \brief Get the number of steel layers in this sector
 * \return copy of Sector::_nsteel
 **/
inline
size_t Sector::nsteel() const
{
    return _nsteel;
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
 * \brief Get the foam thickness
 * \return reference to Sector::_foam_thick
 **/
inline
const double& Sector::foam_thick() const
{
    return _foam_thick;
}


/**
 * \brief Get the steel thickness
 * \return reference to Sector::_steel_thick
 **/
inline
const double& Sector::steel_thick() const
{
    return _steel_thick;
}



} // namespace clas12::geometry::preshower_cal
} // namespace clas12::geometry
} // namespace clas12

#endif // CLAS12_GEOMETRY_PRESHOWER_CAL_SECTOR_HPP
