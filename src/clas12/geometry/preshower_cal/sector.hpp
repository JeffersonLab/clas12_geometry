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

#include "view.hpp"

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
 * \brief a sector of the PCal which consists of several views
 *
 * There are six sectors of PCal in CLAS12 which hold
 * three views
 **/
class Sector
{
  public:
    // inline methods
    const PreshowerCal& pcal() const;

    const vector<unique_ptr<View>>& views() const;
    const View& view(int v) const;
    const View& view(const string& v) const;

    const size_t& index() const;
    string view_name(size_t id) const;

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

    /// \brief a sector consists of several views
    vector<unique_ptr<View>> _views;

    // private inline methods
    size_t view_index(int idx) const;
    size_t view_index(const string& id) const;


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

/** \fn Sector::views()
 * \brief Get a vector of the views in this sector
 * \return const reference to Sector::_views
 **/
inline const vector<unique_ptr<View>>& Sector::views() const
{
    return _views;
}

/**
 * \brief Get a view in this sector
 * \param [in] v The view index within this sector (counting from zero)
 * \return const reference to Sector::_views[v]
 **/
inline const View& Sector::view(int v) const
{
    return *_views[this->view_index(v)];
}

/**
 * \brief Get a view in this sector
 * \param [in] v The view name within this sector (u, v or w)
 * \return const reference to Sector::_views[v]
 **/
inline const View& Sector::view(const string& v) const
{
    return *_views[this->view_index(v)];
}

/**
 * \brief Get a index of this sector in the parent region
 * \return const reference to Sector::_idx
 **/
inline
const size_t& Sector::index() const
{
    return _idx;
}

/**
 * \brief convert negative indexes to positive counting from end
 * \param [in] idx index either from zero or from -1 (counting from end)
 * \return unsigned int index of the region in this sector
 **/
inline
size_t Sector::view_index(int idx) const
{
    return idx<0 ? (_views.size()+idx) : idx;
}

/**
 * \brief convert negative indexes to positive counting from end
 * \param [in] idx index either from zero or from -1 (counting from end)
 * \return unsigned int index of the view in this sector
 **/
inline
size_t Sector::view_index(const string& id) const
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
string Sector::view_name(size_t id) const
{
    static const map<size_t,string> view_index_map {
        {size_t(0), "1a"},
        {1, "1b"},
        {2, "2" } };

    return view_index_map.at(id);
}


} // namespace clas12::geometry::preshower_cal
} // namespace clas12::geometry
} // namespace clas12

#endif // CLAS12_GEOMETRY_PRESHOWER_CAL_SECTOR_HPP
