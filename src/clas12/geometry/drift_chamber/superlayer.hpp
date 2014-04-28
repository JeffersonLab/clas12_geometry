#ifndef CLAS12_GEOMETRY_DRIFT_CHAMBER_SUPERLAYER_HPP
#define CLAS12_GEOMETRY_DRIFT_CHAMBER_SUPERLAYER_HPP

#include <cstddef>
#include <memory>
#include <vector>

#include "geometry/direction_vector.hpp"

#include "senselayer.hpp"
#include "guardlayer.hpp"

namespace clas12
{
namespace geometry
{

class DriftChamber;

namespace drift_chamber
{

using std::unique_ptr;
using std::vector;

using ::geometry::direction_vector;

class Region;

/**
 * \brief Superlayers of wire-planes contained within a single region.
 *
 * This consists of several senselayers surrounded by guard layers.
 *
 * the angles Region::thtilt() and thmin() are used in conjunction
 * with Region::dist2tgt() to get the wire position midpoints:
 * \image html dc.png "Superlayer position determination"
 * \image latex dc.eps "Superlayer position determination"
 *
 * The wires in a given superlayer include guard, field and
 * sense wires as indicated here:
 * \image html superlayer_wires.png "Wires, wire planes and volumes"
 * \image latex superlayer_wires.eps "Wires, wire planes and volumes"
 *
 * \warning the guard layers (front and back) are stored in a single
 * vector of Guardlayer objects. This only makes sense if there
 * will always only be one in front and one in back. If this should
 * change, consider splitting these into two vectors of Guardlayers
 * maybe called front_guardlayers and back_guardlayers.
 **/
class Superlayer
{
  public:
    // inline methods
    const Region& region() const;

    const vector<unique_ptr<Senselayer>>& senselayers() const;
    const Senselayer& senselayer(int lyr) const;
    const vector<unique_ptr<Guardlayer>>& guardlayers() const;
    const Guardlayer& guardlayer(int lyr) const;

    size_t nfieldlayers() const;
    const double& thmin() const;
    const double& wpdist() const;
    const double& cellthickness() const;
    const double& thster() const;

    double layer_thickness() const;

    // methods in cpp file
    size_t nwireplanes() const;
    size_t nlayers() const;
    double thickness() const;
    double dist2tgt() const;
    double first_wire_mid_dist2tgt() const;
    double first_wire_mid_x() const;
    double first_wire_mid_z() const;
    double wire_mid_spacing() const;
    double wire_dircos_x() const;
    double wire_dircos_y() const;
    double wire_dircos_z() const;

    direction_vector<double,3> wire_direction(coordsys_t coordsys=COORDSYS::SECTOR) const;

  private:
    Superlayer(const Region* region, size_t idx);
    Superlayer(const Superlayer& that, const Region* region, size_t idx);

    /// \brief a pointer to the parent region
    const Region* _region;

    /// \brief the index of the superlayer in the vector<Superlayer>
    /// object held by the Region parent class
    size_t _idx;

    /// \brief the sense layers in this superlayer
    vector<unique_ptr<Senselayer>> _senselayers;

    /// \brief the guard layers surrounding the senselayers
    vector<unique_ptr<Guardlayer>> _guardlayers;

    /// \brief number of field wire planes between each
    /// sense or guard layer
    size_t _nfieldlayers;

    /// \brief polar angle to the first guard wire's "mid-point" where
    /// the wire mid-point is the intersection of the wire with
    /// the chamber mid-plane (radians)
    double _thmin;

    /// \brief stereo angle of the wires in the superlayers, angle of
    /// rotation about the normal to the wire plane (radians)
    double _thster;

    /// \brief distance between adjacent wire planes
    /// including sense, guard and field wire planes
    double _wpdist;

    /// \brief average thickness of hexagonal cell in units of _wpdist
    double _cellthickness;

    // private inline methods
    size_t guardlayer_index(int idx) const;
    size_t senselayer_index(int idx) const;

    /// \brief deleted copy constructor
    Superlayer(const Superlayer&) = delete;

    /// \brief deleted assignment operator
    Superlayer& operator=(const Superlayer&) = delete;

    /// \brief the top-level DC class has control of all
    /// private members of this class.
    friend class ::clas12::geometry::DriftChamber;

    /// \brief the "parent" class Region takes care of
    /// copying this class when the top-level DC class's
    /// copy constructor is called.
    friend class Region;
};

/**
 * \brief get the parent region object
 * \return constant reference to Superlayer::_region
 **/
inline
const Region& Superlayer::region() const
{
    return *_region;
}

/**
 * \brief Get a vector of the senselayers in this superlayer
 * \return const reference to Superlayer::_senselayers
 **/
inline
const vector<unique_ptr<Senselayer>>& Superlayer::senselayers() const
{
    return _senselayers;
}

/**
 * \brief Get a senselayer in this superlayer
 * \param [in] lyr The sense layer index within this superlayer (counting from zero)
 * \return const reference to Superlayer::_senselayers[lyr]
 **/
inline
const Senselayer& Superlayer::senselayer(int lyr) const
{
    return *_senselayers[this->senselayer_index(lyr)];
}

/**
 * \brief Get a vector of the guardlayers in this superlayer
 * \return const reference to Superlayer::_senselayers
 **/
inline
const vector<unique_ptr<Guardlayer>>& Superlayer::guardlayers() const
{
    return _guardlayers;
}

/**
 * \brief Get a guardlayer in this superlayer
 * \param [in] lyr The guard layer index within this superlayer (counting from zero)
 * \return const reference to Superlayer::_senselayers[lyr]
 **/
inline
const Guardlayer& Superlayer::guardlayer(int lyr) const
{
    return *_guardlayers[this->guardlayer_index(lyr)];
}


/**
 * \brief Get the number of field layers in this superlayer
 * \return const reference to Superlayer::_nfieldlayers
 **/
inline
size_t Superlayer::nfieldlayers() const
{
    return _nfieldlayers;
}

/**
 * \brief Get the polar angle to the first guard wire's "mid-point"
 *
 * This mid-point is where the wire mid-point is the intersection
 * of the wire with the chamber mid-plane.
 *
 * \return const reference to Superlayer::_thmin (radians)
 **/
inline
const double& Superlayer::thmin() const
{
    return _thmin;
}

/**
 * \brief Get the stereo angle of the wires
 *
 * This is the angle of rotation about the normal to the wire plane.
 *
 * \return const reference to Superlayer::_thster (radians)
 **/
inline
const double& Superlayer::thster() const
{
    return _thster;
}

/**
 * \brief Get the distance between wire planes
 *
 * This includes all sense, guard and field wire planes.
 *
 * \return const reference to Superlayer::_wpdist
 **/
inline
const double& Superlayer::wpdist() const
{
    return _wpdist;
}

/**
 * \brief Get the average thickness of hexagonal cell
 *
 * Distance is in units of Superlayer::wpdist()
 *
 * \return const reference to Superlayer::_cellthickness
 **/
inline
const double& Superlayer::cellthickness() const
{
    return _cellthickness;
}

/**
 * \brief Get the thickness of a hexagonal cell-layer
 *
 * This is the wire-plane thickness times Superlayer::cellthickness()
 * which is the number of wire-planes that make up a hexagonal cell.
 *
 * \return thickness
 **/
inline
double Superlayer::layer_thickness() const
{
    return _cellthickness * _wpdist;
}

/**
 * \brief convert negative indexes to positive counting from end
 * \param [in] idx index either from zero or from -1 (counting from end)
 * \return unsigned int index of the guard layer in this superlayer
 **/
inline
size_t Superlayer::guardlayer_index(int idx) const
{
    return idx<0 ? (_guardlayers.size()+idx) : idx;
}

/**
 * \brief convert negative indexes to positive counting from end
 * \param [in] idx index either from zero or from -1 (counting from end)
 * \return unsigned int index of the sense layer in this superlayer
 **/
inline
size_t Superlayer::senselayer_index(int idx) const
{
    return idx<0 ? (_senselayers.size()+idx) : idx;
}

} // namespace clas12::geometry::drift_chamber
} // namespace clas12::geometry
} // namespace clas12

#endif // CLAS12_GEOMETRY_DRIFT_CHAMBER_SUPERLAYER_HPP

