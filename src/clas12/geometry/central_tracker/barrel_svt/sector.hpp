#ifndef __CLAS12_GEOMETRY_CENTRAL_TRACKER_BARREL_SVT_SECTOR_HPP__
#define __CLAS12_GEOMETRY_CENTRAL_TRACKER_BARREL_SVT_SECTOR_HPP__

#include <iostream>
using std::clog;
using std::endl;

#include <cmath>
#include <cstddef>
#include <memory>
#include <vector>

#include "geometry/plane.hpp"

#include "layer.hpp"

namespace clas12
{
namespace geometry
{
namespace central_tracker
{

class BarrelSVT;

namespace barrel_svt
{

using std::sin;
using std::cos;
using std::unique_ptr;
using std::vector;

using ::geometry::euclid_vector;
using ::geometry::direction_vector;
using ::geometry::line;
using ::geometry::line_segment;
using ::geometry::plane;

class Region;

/**
 * \brief A sector of layers (typically two) in a region
 * of the barrel SVT (BST)
 **/
class Sector
{
  public:
    // inline methods
    const Region& region() const;

    const vector<unique_ptr<Layer>>& layers() const;
    const Layer& layer(const size_t& lyr) const;

    const double& layergap() const;
    const double& fillerthick() const;

    // methods in cpp file
    plane<double> detector_plane() const;
    double phi() const;

    euclid_vector<double,3>    sector_to_clas(const euclid_vector<double,3>& v   ) const;
    direction_vector<double,3> sector_to_clas(const direction_vector<double,3>& v) const;
    line<double,3>             sector_to_clas(const line<double,3>& l            ) const;
    line_segment<double,3>     sector_to_clas(const line_segment<double,3>& l    ) const;
    plane<double>              sector_to_clas(const plane<double>& p             ) const;

  private:
    Sector(const Region* region, const size_t& idx);
    Sector(const Sector& that, const Region* region, const size_t& idx);

    /// \brief a pointer to the parent Region
    const Region* _region;

    /// \brief the index of the sector in the vector<Sector>
    /// object held by the Region parent class
    size_t _idx;

    /// \brief a sector consists of several layers
    vector<unique_ptr<Layer>> _layers;

    /// \brief  distance between 2 layers of each region
    double _layergap;

    /// \brief filler thickness (mm)
    double _fillerthick;

    /*
    /// \brief
    //euclid_vector<double,3> TransformToLAB(const euclid_vector<double,3>& point , const double& gapfactor) const;
     */
    /// \brief deleted copy constructor
    Sector(const Sector&) = delete;

    /// \brief deleted assignment operator
    Sector& operator=(const Sector&) = delete;

    /// \brief the top-level SVT class has control of all
    /// private members of this class.
    friend class ::clas12::geometry::central_tracker::BarrelSVT;

    /// \brief the "parent" class Region takes care of
    /// copying this class when the top-level SVT class's
    /// copy constructor is called.
    friend class ::clas12::geometry::central_tracker::barrel_svt::Region;
};

/**
 * \brief get the parent Region object
 * \return constant reference to Sector::_region
 **/
inline
const Region& Sector::region() const
{
    return *_region;
}

/**
 * \brief Get a vector of layers in this sector
 * \return const reference to Sector::_layers
 **/
inline
const vector<unique_ptr<Layer>>& Sector::layers() const
{
    return _layers;
}

/**
 * \brief Get a layer in this sector
 * \param [in] lyr the layer index in this sector (starting from zero)
 * \return const reference to Sector::_layers[lyr]
 **/
inline
const Layer& Sector::layer(const size_t& lyr) const
{
    return *_layers[lyr];
}

/**
 * \brief Get the distance between 2 layers of each region (mm)
 * \return const reference to Region::_layergap
 **/
inline
const double& Sector::layergap() const
{
    return _layergap;
}

/**
 * \brief Get filler thickness
 * \return const reference to Sector::_fillerthick
 **/
inline
const double& Sector::fillerthick() const
{
    return _fillerthick;
}



} /* namespace clas12::geometry::central_tracker::barrel_svt */
} /* namespace clas12::geometry::central_tracker */
} /* namespace clas12::geometry */
} /* namespace clas12 */

#endif /* __CLAS12_GEOMETRY_CENTRAL_TRACKER_BARREL_SVT_SECTOR_HPP__ */

