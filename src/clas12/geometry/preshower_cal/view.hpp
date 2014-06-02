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

class Layer;

/** \class View
 * \brief A view of strips in a layer of the preshower calorimeter
 *
 * There are three such views in each layer of PCAL
 **/
class View
{
  public:
    // inline methods
    const Layer& layer() const;

    const vector<bool>& strips() const;
    bool strip(int p) const;

    size_t nstrips() const;
    double scint_width(int p) const;
    double scint_max_length() const;


    // methods in cpp file
    string name() const;

  private:
    View(const Layer* layer, size_t idx);
    View(const View& that, const Layer* layer, size_t idx);

    /// \brief a pointer to the parent layer
    const Layer* _layer;

    /// \brief the index of the view in the vector<View>
    /// object held by the Layer parent class
    size_t _idx;


    /// \brief the strips in this view are either on or off
    vector<bool> _strips;

    /// \brief strip width
    vector<double> _scint_width;

    /// \brief maximum strip length
    double _scint_max_length;


    // private inline methods

    size_t strip_index(int idx) const;

    /// \brief deleted copy constructor
    View(const View&) = delete;

    /// \brief deleted assignment operator
    View& operator=(const View&) = delete;

    /// \brief the top-level PCal class has control of all
    /// private members of this class.
    friend class ::clas12::geometry::PreshowerCal;

    /// \brief the "parent" class Layer takes care of
    /// copying this class when the top-level PCal class's
    /// copy constructor is called.
    friend class Layer;
};


/**
 * \brief get the parent layer object
 * \return constant reference to View::_layer
 **/
inline
const Layer& View::layer() const
{
    return *_layer;
}

/**
 * \brief Get a vector of strips in this view
 * \return const reference to View::_strips
 **/
inline
const vector<bool>& View::strips() const
{
    return _strips;
}

/**
 * \brief Get the status of (on/off) of a strip in this view
 * \param [in] p a strip in this view (counting from zero)
 * \return copy of View::_strip[p]
 **/
inline
bool View::strip(int p) const
{
    return _strips[strip_index(p)];
}

/**
 * \brief Get the width of strips in this view
 * \param [in] p a strip in this view (counting from zero)
 * \return copy of View::_strip_width[p]
 **/
inline
double View::scint_width(int p) const
{
    return _scint_width[strip_index(p)];
}

/**
 * \brief Get the maximum length of strips in this view
 * \return copy of View::max_strip_width
 **/
inline
double View::scint_max_length() const
{
    return _scint_max_length;
}


/**
 * \brief Get the number of strips in this view
 * \return copy of View::strips.size()
 **/
inline
size_t View::nstrips() const
{
    return _strips.size();
}

/**
 * \brief convert negative indexes to positive counting from end
 * \param [in] p is index either from zero or from -1 (counting from end)
 * \return unsigned int index of the strip in this view
 **/
inline
size_t View::strip_index(int p) const
{
    return p<0 ? (_strips.size()+p) : p;
}


} // namespace clas12::geometry::preshower_cal
} // namespace clas12::geometry
} // namespace clas12

#endif // CLAS12_GEOMETRY_PRESHOWER_CAL_VIEW_HPP
