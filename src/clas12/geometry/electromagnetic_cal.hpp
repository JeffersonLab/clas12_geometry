#ifndef CLAS12_GEOMETRY_ELECTROMAGNETIC_CAL_HPP
#define CLAS12_GEOMETRY_ELECTROMAGNETIC_CAL_HPP

#include <cstddef>
#include <memory>
#include <string>
#include <vector>

#include "CCDB/Calibration.h"

#include "electromagnetic_cal/sector.hpp"

namespace clas12
{
namespace geometry
{

using std::string;
using std::unique_ptr;
using std::vector;

using ::ccdb::Calibration;

typedef electromagnetic_cal::Sector ECSector;

/**
 * \brief The electromagnetic calorimeter geometry class for CLAS12
 *
 * The EC consists of six sectors, each with 13 layers.
 * Each layer consists of 3 views: U, V, and W, with 36 scintillators each

 **/
class ElectromagneticCal
{
  public:
    ElectromagneticCal();
    ElectromagneticCal(const ElectromagneticCal& that);
    ElectromagneticCal( Calibration* calib,
                const bool& quiet = false,
                const bool& verbose = false );
    ElectromagneticCal& operator=(const ElectromagneticCal& that);

    // inline members
    const vector<unique_ptr<ECSector>>& sectors() const;
    const ECSector& sector(const size_t& sec) const;

    // members in cpp file
    void fetch_nominal_parameters(Calibration* calib);

  private:
    /// \brief the sectors of EC
    vector<unique_ptr<ECSector>> _sectors;

};

/**
 * \brief Get a vector of the sectors in EC
 * \return const reference to ElectromagneticCal::_sectors
 **/
inline
const vector<unique_ptr<ECSector>>& ElectromagneticCal::sectors() const
{
    return _sectors;
}

/**
 * \brief Get a sector in EC
 * \param [in] sec The sector index within the EC (counting from zero)
 * \return const reference to ElectromagneticCal::_sectors[sec]
 **/
inline
const ECSector& ElectromagneticCal::sector(const size_t& sec) const
{
    return *_sectors[sec];
}

} // namespace clas12::geometry
} // namespace clas12

#endif // CLAS12_GEOMETRY_ELECTROMAGNETIC_CAL_HPP
