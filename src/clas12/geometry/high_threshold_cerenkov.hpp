#ifndef CLAS12_GEOMETRY_HIGH_THRESHOLD_CERENKOV_HPP
#define CLAS12_GEOMETRY_HIGH_THRESHOLD_CERENKOV_HPP

#include <cstddef>
#include <memory>
#include <string>
#include <vector>

#include "CCDB/Calibration.h"

#include "high_threshold_cerenkov/sector.hpp"

namespace clas12
{
namespace geometry
{

using std::string;
using std::unique_ptr;
using std::vector;

using ::ccdb::Calibration;

typedef high_threshold_cerenkov::Sector HTCCSector;

/**
 * \brief The high threshold cerenkov geometry class for CLAS12
 *
 * The DC consists of six sectors, each with three regions.
 * Each region consists of two superlayers which hold the 22
 * wire-planes: There are six sense-wire planes in a superlayer
 * surrounded by two guard-wire planes, and between each of these
 * there are two "field-wire" planes.
 **/
class HighThresholdCerenkov
{
  public:
    HighThresholdCerenkov();
    HighThresholdCerenkov(const HighThresholdCerenkov& that);
    HighThresholdCerenkov( Calibration* calib,
                  bool quiet = false,
                  bool verbose = false );
    HighThresholdCerenkov& operator=(const HighThresholdCerenkov& that);

    // inline members
    const vector<unique_ptr<HTCCSector>>& sectors() const;
    const HTCCSector& sector(size_t sec) const;

    // members in cpp file
    void fetch_nominal_parameters(Calibration* calib);

  private:
    /// \brief the sectors of the DC
    vector<unique_ptr<HTCCSector>> _sectors;

};

/**
 * \brief Get a vector of the sectors in the high threshold cerenkov
 * \return const reference to HighThresholdCerenkov::_sectors
 **/
inline
const vector<unique_ptr<HTCCSector>>& HighThresholdCerenkov::sectors() const
{
    return _sectors;
}

/**
 * \brief Get a sector in the DC
 * \param [in] sec The sector index within the high threshold cerenkov (counting from zero)
 * \return const reference to HighThresholdCerenkov::_sectors[sec]
 **/
inline
const HTCCSector& HighThresholdCerenkov::sector(size_t sec) const
{
    return *_sectors[sec];
}

} // namespace clas12::geometry
} // namespace clas12

#endif // CLAS12_GEOMETRY_HIGH_THRESHOLD_CERENKOV_HPP
