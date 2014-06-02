#ifndef CLAS12_GEOMETRY_PRESHOWER_CAL_HPP
#define CLAS12_GEOMETRY_PRESHOWER_CAL_HPP

#include <cstddef>
#include <memory>
#include <string>
#include <vector>

#include "CCDB/Calibration.h"

#include "preshower_cal/sector.hpp"

namespace clas12
{
namespace geometry
{

using std::string;
using std::unique_ptr;
using std::vector;

using ::ccdb::Calibration;

typedef preshower_cal::Sector PCalSector;

/**
 * \brief The preshower calorimeter geometry class for CLAS12
 *
 * The PCAL consists of six sectors, each of
 * which with 5 layers.
 * Each layer consists of 3 views: U, V, and W, with a different number of scintillators
 * schintillators: 68 for u, 62 for v and w
 **/
class PreshowerCal
{
  public:
    PreshowerCal();
    PreshowerCal(const PreshowerCal& that);
    PreshowerCal( Calibration* calib,
                const bool& quiet = false,
                const bool& verbose = false );
    PreshowerCal& operator=(const PreshowerCal& that);

    // inline members
    const vector<unique_ptr<PCalSector>>& sectors() const;
    const PCalSector& sector(const size_t& sec) const;

    // members in cpp file
    void fetch_nominal_parameters(Calibration* calib);

  private:
    /// \brief the sectors of PCAL
    vector<unique_ptr<PCalSector>> _sectors;

};

/**
 * \brief Get a vector of the sectors in PCAL
 * \return const reference to PreshowerCal::_sectors
 **/
inline
const vector<unique_ptr<PCalSector>>& PreshowerCal::sectors() const
{
    return _sectors;
}

/**
 * \brief Get a sector in PCAL
 * \param [in] sec The sector index within the PCAL (counting from zero)
 * \return const reference to PreshowerCal::_sectors[sec]
 **/
inline
const PCalSector& PreshowerCal::sector(const size_t& sec) const
{
    return *_sectors[sec];
}

} // namespace clas12::geometry
} // namespace clas12

#endif // CLAS12_GEOMETRY_PRESHOWER_CAL_HPP
