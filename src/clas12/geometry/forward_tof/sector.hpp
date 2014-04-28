#ifndef CLAS12_GEOMETRY_FORWARD_TOF_SECTOR_HPP
#define CLAS12_GEOMETRY_FORWARD_TOF_SECTOR_HPP

#include <iostream>
using std::clog;
using std::endl;

#include <cstddef>
#include <memory>
#include <vector>
#include <string>
#include <map>

#include "panel.hpp"

namespace clas12
{
namespace geometry
{

class ForwardTOF;

namespace forward_tof
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

using ::clas12::geometry::ForwardTOF;

/**
 * \brief a sector of the FTOF which consists of several panels
 *
 * There are six sectors of FTOF in CLAS12 which hold
 * three panels
 **/
class Sector
{
  public:
    // inline methods
    const ForwardTOF& ftof() const;

    const vector<unique_ptr<Panel>>& panels() const;
    const Panel& panel(const int& pan) const;
    const Panel& panel(const string& pan) const;

    const size_t& index() const;
    string panel_name(size_t id) const;

    euclid_vector<double,3>    sector_to_clas(const euclid_vector<double,3>& v   ) const;
    direction_vector<double,3> sector_to_clas(const direction_vector<double,3>& v) const;
    line<double,3>             sector_to_clas(const line<double,3>& l            ) const;
    line_segment<double,3>     sector_to_clas(const line_segment<double,3>& l    ) const;
    plane<double>              sector_to_clas(const plane<double>& p             ) const;


  private:
    Sector(const ForwardTOF* ftof, const size_t& idx);
    Sector(const Sector& that, const ForwardTOF* ftof, const size_t& idx);

    /// \brief a pointer to the parent sector
    const ForwardTOF* _ftof;

    /// \brief the index of the sector in the vector<Sector>
    /// object held by the ForwardTOF parent class
    size_t _idx;

    /// \brief a sector consists of several panels
    vector<unique_ptr<Panel>> _panels;

    // private inline methods
    size_t panel_index(const int& idx) const;
    size_t panel_index(const string& id) const;


    /// \brief deleted copy constructor
    Sector(const Sector&) = delete;

    /// \brief deleted assignment operator
    Sector& operator=(const Sector&) = delete;

    /// \brief the top-level FTOF class has control of all
    /// private members of this class.
    friend class ::clas12::geometry::ForwardTOF;
};

/**
 * \brief get the parent ftof object
 * \return constant reference to Sector::_ftof
 **/
inline const ForwardTOF& Sector::ftof() const
{
    return *_ftof;
}

/** \fn Sector::panels()
 * \brief Get a vector of the panels in this sector
 * \return const reference to Sector::_panels
 **/
inline const vector<unique_ptr<Panel>>& Sector::panels() const
{
    return _panels;
}

/**
 * \brief Get a panel in this sector
 * \param [in] pan The panel index within this sector (counting from zero)
 * \return const reference to Sector::_panels[pan]
 **/
inline const Panel& Sector::panel(const int& pan) const
{
    return *_panels[this->panel_index(pan)];
}

/**
 * \brief Get a panel in this sector
 * \param [in] pan The panel name within this sector (1a, 1b or 2)
 * \return const reference to Sector::_panels[pan]
 **/
inline const Panel& Sector::panel(const string& pan) const
{
    return *_panels[this->panel_index(pan)];
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
size_t Sector::panel_index(const int& idx) const
{
    return idx<0 ? (_panels.size()+idx) : idx;
}

/**
 * \brief convert negative indexes to positive counting from end
 * \param [in] idx index either from zero or from -1 (counting from end)
 * \return unsigned int index of the panel in this sector
 **/
inline
size_t Sector::panel_index(const string& id) const
{
    static const map<string,size_t> panel_index_map {
        {"1a", 0},
        {"1b", 1},
        {"2",  2} };

    size_t pos = id.find("panel");
    if (pos == string::npos)
    {
        return panel_index_map.at(id);
    }
    else
    {
        return panel_index_map.at(id.substr(string("panel").size(), string::npos));
    }
}

/**
 * \brief convert negative indexes to positive counting from end
 * \param [in] idx index either from zero or from -1 (counting from end)
 * \return name of the panel
 **/
inline
string Sector::panel_name(size_t id) const
{
    static const map<size_t,string> panel_index_map {
        {size_t(0), "1a"},
        {1, "1b"},
        {2, "2" } };

    return panel_index_map.at(id);
}


} // namespace clas12::geometry::forward_tof
} // namespace clas12::geometry
} // namespace clas12

#endif // CLAS12_GEOMETRY_FORWARD_TOF_SECTOR_HPP
