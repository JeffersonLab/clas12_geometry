#ifndef __CLAS12_GEOMETRY_CENTRAL_TRACKER_BARREL_SVT_LAYER_HPP__
#define __CLAS12_GEOMETRY_CENTRAL_TRACKER_BARREL_SVT_LAYER_HPP__

#include <iostream>
using std::clog;
using std::endl;

#include <cmath>
#include <cstddef>
#include <memory>
#include <vector>


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
using ::geometry::line_segment;

class Sector;

/**
 * \brief A layer of layers in a sector of the barrel SVT (BST)
 **/
class Layer
{
  public:
    // inline methods
    const Sector& sector() const;

    const vector<bool>& strips() const;
    bool strip(const size_t& idx) const;

    const double& readoutPitch() const;
    const double& siliconWidth() const;
    const double& physSenLen() const;
    const double& physSenWid() const;
    const double& activeSenLen() const;
    const double& activeSenWid() const;
    const double& deadZnSenLen1() const;
    const double& deadZnSenLen2() const;
    const double& deadZnSenLen3() const;
    const double& deadZnSenWid() const;
    const double& startAngle() const;
    const double& endAngle() const;

    size_t nstrips() const;
    size_t strip_index(const int& s) const;

    // methods in cpp file
    double radius() const;
    euclid_vector<double,3> stripFirstPoint(const int& is) const;
    euclid_vector<double,3> stripSecondPoint(const int& is) const;
    line_segment<double,3> siliconStrip(const int& is) const;
    vector<line_segment<double,3>> siliconStrips() const;
    //double active_zstart() const;

  private:
    Layer(const Sector* sector, const size_t& idx);
    Layer(const Layer& that, const Sector* sector, const size_t& idx);

    /// \brief a pointer to the parent Sector
    const Sector* _sector;

    /// \brief the index of the layer in the vector<Layer>
    /// object held by the Sector parent class
    size_t _idx;

    /// \brief the sense strips in this layer/module
    vector<bool> _strips;

    /// \brief BST readout pitch (mm)
    double _readoutPitch;

    /// \brief BST silicon width (mm)
    double _siliconWidth;

    /// \brief  physical length of a sensor (mm)
    double _physSenLen;

    /// \brief  physical width of a sensor (mm)
    double _physSenWid;

    /// \brief  active length of a sensor (mm)
    double _activeSenLen;

    /// \brief  active width of a sensor (mm)
    double _activeSenWid;

    /// \brief  distance between the pitch adapter
    /// and the active part of the hybrid sensor (mm)
    double _deadZnSenLen1;

    /// \brief  distance between the active parts of
    /// the hybrid and intermediate sensors
    double _deadZnSenLen2;

    /// \brief  distance between the active parts of
    /// the intermediate and far sensors (mm)
    double _deadZnSenLen3;

    /// \brief vertical distance between the active
    /// part of any sensor and the bottom of a strip (mm)
    double _deadZnSenWid;

    /// \brief angle of the first readout strip (deg)
    double _startAngle;

    /// \brief angle of the last readout strip (deg)
    double _endAngle;

    /// \brief
    euclid_vector<double,3> TransformToLAB(const euclid_vector<double,3>& point , const double& gapfactor) const;

    /// \brief deleted copy constructor
    Layer(const Layer&) = delete;

    /// \brief deleted assignment operator
    Layer& operator=(const Layer&) = delete;

    /// \brief the top-level SVT class has control of all
    /// private members of this class.
    friend class ::clas12::geometry::central_tracker::BarrelSVT;

    /// \brief the "parent" class Sector takes care of
    /// copying this class when the top-level SVT class's
    /// copy constructor is called.
    friend class ::clas12::geometry::central_tracker::barrel_svt::Sector;
};

/**
 * \brief get the parent Sector object
 * \return constant reference to Layer::_sector
 **/
inline
const Sector& Layer::sector() const
{
    return *_sector;
}

/**
 * \brief Get a vector of strips in this layer
 * \return const reference to Layer::_strips
 **/
inline
const vector<bool>& Layer::strips() const
{
    return _strips;
}

/**
 * \brief Get the status of (on/off) of a strip in this layer
 * \param [in] idx a strip index in this layer (counting from zero)
 * \return copy of Layer::_strips[idx]
 **/
inline
bool Layer::strip(const size_t& idx) const
{
    return _strips[idx];
}

/**
 * \brief Get the readout pitch
 *
 * This is the distance from one strip to the next on the read-out
 * side of the boards.
 *
 * \return const reference to Layer::_readoutPitch
 **/
inline
const double& Layer::readoutPitch() const
{
    return _readoutPitch;
}

/**
 * \brief Get the silicon width
 *
 * This is the silicon sensor width
 *
 * \return const reference to Layer::_siliconWidth
 **/
inline
const double& Layer::siliconWidth() const
{
    return _siliconWidth;
}


/**
 * \brief Get the  physical length of a sensor (mm)
 * \return const reference to Layer::_physSenLen
 **/
inline
const double& Layer::physSenLen() const
{
    return _physSenLen;
}

/**
 * \brief Get the  physical width of a sensor (mm)
 * \return const reference to Layer::_physSenWid
 **/
inline
const double& Layer::physSenWid() const
{
    return _physSenWid;
}


/**
 * \brief Get the active length of a sensor (mm)
 * \return const reference to Layer::_activeSenLen
 **/
inline
const double& Layer::activeSenLen() const
{
    return _activeSenLen;
}

/**
 * \brief Get the  active width of a sensor (mm)
 * \return const reference to Layer::_activeSenWid
 **/
inline
const double& Layer::activeSenWid() const
{
    return _activeSenWid;
}

/**
 * \brief Get deadZnSenLen1
 * which is the distance between the pitch adapter and the active
 * part of the hybrid sensor (mm)
 * \return const reference to Layer::_deadZnSenLen1
 **/
inline
const double& Layer::deadZnSenLen1() const
{
    return _deadZnSenLen1;
}

/**
 * \brief Get deadZnSenLen2
 * which is the distance  between the active parts of the
 * hybrid and intermediate sensors (mm)
 * \return const reference to Layer::_deadZnSenLen2
 **/
inline
const double& Layer::deadZnSenLen2() const
{
    return _deadZnSenLen2;
}

/**
 * \brief Get deadZnSenLen3
 * which is the distance  between the active parts of the
 * intermediate and far sensors (mm)
 * \return const reference to Layer::_deadZnSenLen3
 **/
inline
const double& Layer::deadZnSenLen3() const
{
    return _deadZnSenLen3;
}

/**
 * \brief Get deadZnSenWid
 * which is the vertical distance between the active part of
 * any sensor and the bottom of a strip (mm)
 * \return const reference to Layer::_deadZnSenWid
 **/
inline
const double& Layer::deadZnSenWid() const
{
    return _deadZnSenWid;
}

/**
 * \brief Get startAngle: the angle of the first readout strip (deg)
 * \return const reference to Layer::_startAngle
 **/
inline
const double& Layer::startAngle() const
{
    return _startAngle;
}

/**
 * \brief Get endAngle: the angle of the last readout strip (deg)
 * \return const reference to Layer::_endAngle
 **/
inline
const double& Layer::endAngle() const
{
    return _endAngle;
}

/**
 * \brief Get the number of total strips in this layer
 *
 * \return Layer::_strips.size()
 **/
inline
size_t Layer::nstrips() const
{
    return _strips.size();
}

/**
 * \brief convert negative strip index to positive
 * where -1 is the last strip index in this layer.
 * \param [in] idx index either from zero or from -1 (counting from end)
 * \return unsigned int: the strip index
 **/
inline
size_t Layer::strip_index(const int& s) const
{
    return s < 0 ? (this->nstrips()+s) : s;
}

} /* namespace clas12::geometry::central_tracker::barrel_svt */
} /* namespace clas12::geometry::central_tracker */
} /* namespace clas12::geometry */
} /* namespace clas12 */

#endif /* __CLAS12_GEOMETRY_CENTRAL_TRACKER_BARREL_SVT_LAYER_HPP__ */

