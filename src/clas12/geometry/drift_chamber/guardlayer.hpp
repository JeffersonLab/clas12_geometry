#ifndef CLAS12_GEOMETRY_DRIFT_CHAMBER_GUARDLAYER_HPP
#define CLAS12_GEOMETRY_DRIFT_CHAMBER_GUARDLAYER_HPP

#include <cstddef>

#include "geometry/euclid_vector.hpp"
#include "geometry/line_segment.hpp"
#include "geometry/plane.hpp"

namespace clas12
{
namespace geometry
{

class DriftChamber;

namespace drift_chamber
{

using ::geometry::euclid_vector;
using ::geometry::line_segment;
using ::geometry::plane;

class Superlayer;

/**
 * \brief A layer that consists of only guard wires.
 *
 * There are typically two guard layers in a given superlayer which
 * surround the sense and field wire planes.
 **/
class Guardlayer
{
  public:
    // inline methods
    const Superlayer& superlayer() const;

    size_t nwires() const;

    // methods in cpp file
    double wire_mid_x(int w) const;
    double wire_mid_y(int w=0) const;
    double wire_mid_z(int w) const;
    euclid_vector<double,3> wire_mid(int w, coordsys_t coordsys=COORDSYS::SECTOR) const;

    euclid_vector<double,3> wire_center(int w, coordsys_t coordsys=COORDSYS::SECTOR) const;

    double dist2tgt() const;

    line_segment<double,3> wire(int w, coordsys_t coordsys=COORDSYS::SECTOR) const;
    vector<line_segment<double,3>> wires(coordsys_t coordsys=COORDSYS::SECTOR) const;
    double wire_length(int w) const;

  private:
    Guardlayer(const Superlayer* superlayer, size_t idx);
    Guardlayer(const Guardlayer& that, const Superlayer* superlayer, size_t idx);

    /// \brief a pointer to the parent superlayer
    const Superlayer* _superlayer;

    /// \brief the index of the guardlayer in the vector<Guardlayer>
    /// object held by the Superlayer parent class
    size_t _idx;

    /// \brief number of (guard) wires in this layer
    size_t _nwires;

    // inline methods
    size_t wire_index(int w) const;

    /// \brief deleted copy constructor
    Guardlayer(const Guardlayer&) = delete;

    /// \brief deleted assignment operator
    Guardlayer& operator=(const Guardlayer&) = delete;

    /// \brief the top-level DC class has control of all
    /// private members of this class.
    friend class ::clas12::geometry::DriftChamber;

    /// \brief the "parent" class Superlayer takes care of
    /// copying this class when the top-level DC class's
    /// copy constructor is called.
    friend class Superlayer;
};

/**
 * \brief get the parent superlayer object
 * \return constant reference to Guardlayer::_superlayer
 **/
inline
const Superlayer& Guardlayer::superlayer() const
{
    return *_superlayer;
}

/**
 * \brief Get the number of (guard) wires in this layer
 * \return const reference to Guardlayer::_nwires
 **/
inline
size_t Guardlayer::nwires() const
{
    return _nwires;
}

/**
 * \brief convert negative wire index to positive
 * where -1 is the last wire index in this layer.
 * \param [in] idx index either from zero or from -1 (counting from end)
 * \return unsigned int: the wire index
 **/
inline
size_t Guardlayer::wire_index(int w) const
{
    return w < 0 ? (_nwires+w) : w;
}

} // namespace clas12::geometry::drift_chamber
} // namespace clas12::geometry
} // namespace clas12

#endif // CLAS12_GEOMETRY_DRIFT_CHAMBER_GUARDLAYER_HPP

