#ifndef CLAS12_GEOMETRY_PRESHOWER_CAL_VIEW_HPP
#define CLAS12_GEOMETRY_PRESHOWER_CAL_VIEW_HPP

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
 * \brief A view of layers in a sector of the forward time of flight
 *
 * There are three such views in each sector of CLAS12
 **/
class View
{
  public:
    // inline methods
    const Sector& sector() const;

    const vector<unique_ptr<Layer>>& layers() const;
    const Layer& layer(int l) const;

    size_t nlayers() const;

    // methods in cpp file
    string name() const;

  private:
    View(const Sector* sector, size_t idx);
    View(const View& that, const Sector* sector, size_t idx);

    /// \brief a pointer to the parent sector
    const Sector* _sector;

    /// \brief the index of the view in the vector<View>
    /// object held by the Sector parent class
    size_t _idx;

    /// \brief collection of Layers in this View
    vector<unique_ptr<Layer>> _layers;

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

/** \fn View::layers()
 * \brief Get a vector of the layers in this view
 * \return const reference to View::_layers
 **/
inline const vector<unique_ptr<Layer>>& View::layers() const
{
    return _layers;
}

/**
 * \brief Get the status of (on/off) of a layer in this view
 * \param [in] p a layer in this view (counting from zero)
 * \return copy of View::_layer[p]
 **/
inline
bool View::layer(const int& p) const
{
    return _layers[layer_index(p)];
}

/**
 * \brief Get the number of layers in this view
 * \return copy of View::layers.size()
 **/
inline
size_t View::nlayers() const
{
    return _layers.size();
}

/**
 * \brief convert negative indexes to positive counting from end
 * \param [in] p is index either from zero or from -1 (counting from end)
 * \return unsigned int index of the layer in this view
 **/
inline
size_t View::layer_index(const int& p) const
{
    return p<0 ? (_layers.size()+p) : p;
}

} // namespace clas12::geometry::preshower_cal
} // namespace clas12::geometry
} // namespace clas12

#endif // CLAS12_GEOMETRY_PRESHOWER_CAL_VIEW_HPP
