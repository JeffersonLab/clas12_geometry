#ifndef CLAS12_GEOMETRY_DRIFT_CHAMBER_SENSELAYER_HPP
#define CLAS12_GEOMETRY_DRIFT_CHAMBER_SENSELAYER_HPP

#include <cstddef>
#include <vector>

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

using std::vector;

using ::geometry::euclid_vector;
using ::geometry::line_segment;
using ::geometry::plane;

class Superlayer;

/**
 * \brief A layer that consists of sense and guard wires.
 *
 * There are typically two guard wires which surround many
 * sensewires, each of which can be on or off.
 *
 * \warning the number of guard wires (forward and backward) are stored
 * as a single number. This only makes sense if there will only
 * ever be one at the forward-most angle and one in the backward-most
 * angle (this number will always be equal to 2). If this should
 * change, consider splitting this number into two: nguardwires_forward
 * and nguardwires_backward.
 **/
class Senselayer
{
  public:
    // inline methods
    const Superlayer& superlayer() const;

    const vector<bool>& sensewires() const;
    bool sensewire(int w) const;

    const size_t& nguardwires() const;
    size_t nwires() const;

    // public methods in cpp file
    double wire_mid_x(int w) const;
    double wire_mid_y(int w=0) const;
    double wire_mid_z(int w) const;
    euclid_vector<double,3> wire_mid(int w, coordsys_t coordsys=COORDSYS::SECTOR) const;

    vector<double> wires_mid_x() const;
    vector<double> wires_mid_y() const;
    vector<double> wires_mid_z() const;
    vector< euclid_vector<double,3> > wires_mid(coordsys_t coordsys=COORDSYS::SECTOR) const;

    euclid_vector<double,3> wire_center(int w, coordsys_t coordsys=COORDSYS::SECTOR) const;
    euclid_vector<double,3> center(coordsys_t coordsys=COORDSYS::SECTOR) const;

    double dist2tgt() const;

    line_segment<double,3> wire(int w, coordsys_t coordsys=COORDSYS::SECTOR) const;
    vector<line_segment<double,3>> wires(coordsys_t coordsys=COORDSYS::SECTOR) const;
    double wire_length(int w) const;

    plane<double> wire_plane(coordsys_t coordsys=COORDSYS::SECTOR) const;

  private:
    Senselayer(const Superlayer* superlayer, size_t idx);
    Senselayer(const Senselayer& that, const Superlayer* superlayer, size_t idx);

    /// \brief a pointer to the parent superlayer
    const Superlayer* _superlayer;

    /// \brief the index of the senselayer in the vector<Senselayer>
    /// object held by the Superlayer parent class
    size_t _idx;

    /// \brief the sense wires in this layer are either on or off
    vector<bool> _sensewires;

    /// \brief number of guard wires in this layer
    size_t _nguardwires;

    // inline methods
    size_t wire_index(int w) const;

    /// \brief deleted copy constructor
    Senselayer(const Senselayer&) = delete;

    /// \brief deleted assignment operator
    Senselayer& operator=(const Senselayer&) = delete;

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
 * \return constant reference to Senselayer::_superlayer
 **/
inline
const Superlayer& Senselayer::superlayer() const
{
    return *_superlayer;
}

/**
 * \brief Get a vector of sensewires in a senselayer
 * \return const reference to Senselayer::_sensewires
 **/
inline
const vector<bool>& Senselayer::sensewires() const
{
    return _sensewires;
}

/**
 * \brief Get the status of (on/off) of a wire in this senselayer
 * \param [in] w a wire in this senselayer (counting from zero)
 * \return copy of Senselayer::_sensewire[wire]
 **/
inline
bool Senselayer::sensewire(int w) const
{
    return _sensewires[wire_index(w)];
}

/**
 * \brief Get the number of guard wires in this layer
 *
 * These wires surround the sense wires in this wire-plane
 *
 * \return const reference to Senselayer::_nguardwires
 **/
inline
const size_t& Senselayer::nguardwires() const
{
    return _nguardwires;
}

/**
 * \brief Get the number of total (sense+guard) wires in this layer
 *
 * \return Senselayer::_nguardwires + Senselayer::_sensewires.size()
 **/
inline
size_t Senselayer::nwires() const
{
    return _nguardwires + _sensewires.size();
}

/**
 * \brief convert negative wire index to positive
 * where -1 is the last wire index in this layer.
 * \param [in] idx index either from zero or from -1 (counting from end)
 * \return unsigned int: the wire index
 **/
inline
size_t Senselayer::wire_index(int w) const
{
    return w < 0 ? (this->nwires()+w) : w;
}

} // namespace clas12::geometry::drift_chamber
} // namespace clas12::geometry
} // namespace clas12

#endif // CLAS12_GEOMETRY_DRIFT_CHAMBER_SENSELAYER_HPP

