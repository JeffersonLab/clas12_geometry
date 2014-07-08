#ifndef CLAS12_GEOMETRY_ELECTROMAGNETIC_CAL_LAYER_HPP
#define CLAS12_GEOMETRY_ELECTROMAGNETIC_CAL_LAYER_HPP

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
#include "view.hpp"

namespace clas12
{
namespace geometry
{

class ElectromagneticCal;

namespace electromagnetic_cal
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

/** \class Layer
 * \brief A layer of views in the electromagnetic calorimeter (EC)
 *        There are 13 such layers in each sector of EC
 **/
class Layer
{

  public:
    // inline methods
    const Sector& sector() const;

    const vector<unique_ptr<View>>& views() const;
    const View& view(int v) const;

    size_t nviews() const;
    const double& view_angle() const;
    const double& wrapper_thick() const;
    const double& strip_thick() const;
    const double& lead_thick() const;

    string view_name(size_t id) const;

    // methods in cpp file


  private:
    Layer(const View* view, size_t idx);
    Layer(const Layer& that, const View* view, size_t idx);

    /// \brief a pointer to the parent view
    const View* _view;


 // private inline methods

    Layer(const Sector* sector, size_t idx);
    Layer(const Layer& that, const Sector* sector, size_t idx);

    /// \brief a pointer to the parent sector
    const Sector* _sector;

    /// \brief the index of the layer in the vector<Layer>
    /// object held by the Sector parent class
    size_t _idx;


    ///\brief number of views in this layer
    size_t _nviews;

    /// \brief angle in degrees of the EC vertex closest to the beamline
    double _view_angle;

    /// \brief thickness of the TiO2 reflective wrapping around each strip (mm)
    double _wrapper_thick;

    /// \brief thickness of each scintillator strip (mm)
    double _strip_thick;

    /// \brief thickness of each lead layer (mm)
    double _lead_thick;

    /// \brief collection of Views in this Layer
    vector<unique_ptr<View>> _views;

    /// \brief convert negative index to run from the end of the vector
    size_t view_index(int idx) const;
    size_t view_index(const string& id) const;

    /// \brief deleted copy constructor
    Layer(const Layer&) = delete;

    /// \brief deleted assignment operator
    Layer& operator=(const Layer&) = delete;

    /// \brief the top-level EC class has control of all
    /// private members of this class.
    friend class ::clas12::geometry::ElectromagneticCal;

    /// \brief the "parent" class Sector takes care of
    /// copying this class when the top-level PCal class's
    /// copy constructor is called.
    friend class Sector;
};


/**
 * \brief get the parent sector object
 * \return constant reference to Layer::_sector
 **/
inline
const Sector& Layer::sector() const
{
    return *_sector;
}

/** \fn Layer::views()
 * \brief Get a vector of the views in this layer
 * \return const reference to Layer::_views
 **/
inline const vector<unique_ptr<View>>& Layer::views() const
{
    return _views;
}


/**
 * \brief Get a view in this layer
 * \param [in] v The view name within this layer (u, v or w)
 * \return const reference to Layer::_views[v]
 **/


/**
 * \brief convert negative indexes to positive counting from end
 * \param [in] idx index either from zero or from -1 (counting from end)
 * \return unsigned int index of the region in this sector
 **/
inline
size_t Layer::view_index(int idx) const
{
    return idx<0 ? (_views.size()+idx) : idx;
}

/**
 * \brief convert negative indexes to positive counting from end
 * \param [in] idx index either from zero or from -1 (counting from end)
 * \return unsigned int index of the view in this sector
 **/
inline
size_t Layer::view_index(const string& id) const
{
    static const map<string,size_t> view_index_map {
        {"u", 0},
        {"v", 1},
        {"w",  2} };

    size_t pos = id.find("view");
    if (pos == string::npos)
    {
        return view_index_map.at(id);
    }
    else
    {
        return view_index_map.at(id.substr(string("view").size(), string::npos));
    }
}

/**
 * \brief convert negative indexes to positive counting from end
 * \param [in] idx index either from zero or from -1 (counting from end)
 * \return name of the view
 **/
inline
string Layer::view_name(size_t id) const
{
    static const map<size_t,string> view_index_map {
        {size_t(0), "u"},
        {1, "v"},
        {2, "w" } };

    return view_index_map.at(id);
}





/**
 * \brief Get the status of (on/off) of a view in this layer
 * \param [in] p a view in this layer (counting from zero)
 * \return copy of Layer::_view[p]
 **/
inline
const View& Layer::view(int v) const
{
    return *_views[view_index(v)];
}

/**
 * \brief Get the number of views in this layer
 * \return copy of Layer::views.size()
 **/
inline
size_t Layer::nviews() const
{
    return _views.size();
}




/**
 * \brief Get the view angle
 * \return reference to Layer::_view_angle
 **/
inline
const double& Layer::view_angle() const
{
    return _view_angle;
}


/**
 * \brief Get the wrapper thickness
 * \return reference to Layer::_wrapper_thick
 **/
inline
const double& Layer::wrapper_thick() const
{
    return _wrapper_thick;
}

/**
 * \brief Get the scintillator thickness
 * \return reference to Layer::_strip_thick
 **/
inline
const double& Layer::strip_thick() const
{
    return _strip_thick;
}


/**
 * \brief Get the lead thickness
 * \return reference to Layer::_lead_thick
 **/
inline
const double& Layer::lead_thick() const
{
    return _lead_thick;
}





} // namespace clas12::geometry::electromagnetic_cal
} // namespace clas12::geometry
} // namespace clas12

#endif // CLAS12_GEOMETRY_ELECTROMAGNETIC_CAL_LAYER_HPP
