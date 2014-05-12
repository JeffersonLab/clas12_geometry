#ifndef CLAS12_GEOMETRY_CENTRAL_TRACKER_HPP
#define CLAS12_GEOMETRY_CENTRAL_TRACKER_HPP

#include <cstddef>
#include <string>
#include <vector>

#include "CCDB/Calibration.h"

#include "central_tracker/barrel_svt.hpp"
//#include "central_tracker/foward_svt.hpp"

namespace clas12
{
namespace geometry
{

using std::string;
using std::vector;

using central_tracker::BarrelSVT;
//using central_tracker::ForwardSVT;

using ::ccdb::Calibration;

/**
 * \brief The silicone vertex tracker geometry class for CLAS12
 *
 * The SVT consists of two major components: the Barrel SVT (BST)
 * and the Forward SVT (FST).
 **/
class CentralTracker
{
  public:
    CentralTracker();
    CentralTracker(const CentralTracker& that);
    CentralTracker(Calibration* calib);
    CentralTracker& operator=(const CentralTracker& that);

    // inline members
    const BarrelSVT& bst() const;
    //const ForwardSVT& fst() const;

    // members in cpp file
    void fetch_nominal_parameters(Calibration* calib);

  private:
    /// \brief the barrel SVT (BST)
    BarrelSVT _bst;
    // / \brief the forward SVT (FST)
    //ForwardSVT _fst;

};

/**
 * \brief Get the BarrelSVT (BST) object
 * \return const reference to CentralTracker::_bst
 **/
inline
const BarrelSVT& CentralTracker::bst() const
{
    return _bst;
}

/**
 * \brief Get the ForwardSVT (FST) object
 * \return const reference to CentralTracker::_fst
 **/
//inline
//const ForwardSVT& CentralTracker::fst() const
//{
    //return _fst;
//}

} // namespace clas12::geometry
} // namespace clas12

#endif // CLAS12_GEOMETRY_CENTRAL_TRACKER_HPP
