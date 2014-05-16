#ifndef CLAS12_GEOMETRY_PRESHOWER_CAL_LAYER_HPP
#define CLAS12_GEOMETRY_PRESHOWER_CAL_LAYER_HPP

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

class View;

/** \class Layer
 * \brief A layer of strips in a view of the preshower calorimeter
 **/
class Layer
{
  public:
    // inline methods
    const View& view() const;

    const vector<bool>& strips() const;
    bool strip(int p) const;

    size_t nstrips() const;
    double strip_width(int p) const;

    // methods in cpp file

  private:
    Layer(const View* view, size_t idx);
    Layer(const Layer& that, const View* view, size_t idx);

    /// \brief a pointer to the parent view
    const View* _view;

    /// \brief the index of the view in the vector<View>
    /// object held by the View parent class
    size_t _idx;

    /// \brief the strips in this view are either on or off
    vector<bool> _strips;

    /// \brief strip width
    vector<double> _strip_width;


    // private inline methods
    size_t strip_index(int idx) const;

    /// \brief deleted copy constructor
    Layer(const Layer&) = delete;

    /// \brief deleted assignment operator
    Layer& operator=(const Layer&) = delete;

    /// \brief the top-level PCal class has control of all
    /// private members of this class.
    friend class ::clas12::geometry::PreshowerCal;

    /// \brief the "parent" class View takes care of
    /// copying this class when the top-level PCal class's
    /// copy constructor is called.
    friend class View;
};

/**
 * \brief get the parent view object
 * \return constant reference to View::_view
 **/
inline
const View& Layer::view() const
{
    return *_view;
}

/**
 * \brief Get a vector of strips in this view
 * \return const reference to Layer::_strips
 **/
inline
const vector<bool>& Layer::strips() const
{
    return _strips;
}

/**
 * \brief Get the status of (on/off) of a strip in this view
 * \param [in] p a strip in this view (counting from zero)
 * \return copy of Layer::_strip[p]
 **/
inline
bool Layer::strip(int p) const
{
    return _strips[strip_index(p)];
}

/**
 * \brief Get the width of strips in this view
 * \param [in] p a strip in this view (counting from zero)
 * \return copy of Layer::_strip[p]
 **/
inline
double Layer::strip_width(int p) const
{
    return _strip_width[strip_index(p)];
}


/**
 * \brief Get the number of strips in this view
 * \return copy of Layer::strips.size()
 **/
inline
size_t Layer::nstrips() const
{
    return _strips.size();
}

/**
 * \brief convert negative indexes to positive counting from end
 * \param [in] p is index either from zero or from -1 (counting from end)
 * \return unsigned int index of the strip in this view
 **/
inline
size_t Layer::strip_index(int p) const
{
    return p<0 ? (_strips.size()+p) : p;
}

} // namespace clas12::geometry::preshower_cal
} // namespace clas12::geometry
} // namespace clas12

#endif // CLAS12_GEOMETRY_PRESHOWER_CAL_LAYER_HPP
