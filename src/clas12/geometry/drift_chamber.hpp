#ifndef __CLAS12_GEOMETRY_DRIFT_CHAMBER_HPP__
#define __CLAS12_GEOMETRY_DRIFT_CHAMBER_HPP__

#include <cstddef>
#include <memory>
#include <string>
#include <vector>

#include "drift_chamber/sector.hpp"

namespace clas12
{
namespace geometry
{

using std::string;
using std::unique_ptr;
using std::vector;

typedef drift_chamber::Sector DCSector;

/**
 * \brief The drift chamber geometry class for CLAS12
 *
 * The DC consists of six sectors, each with three regions.
 * Each region consists of two superlayers which hold the 22
 * wire-planes: There are six sense-wire planes in a superlayer
 * surrounded by two guard-wire planes, and between each of these
 * there are two "field-wire" planes.
 **/
class DriftChamber
{
  public:
    DriftChamber();
    DriftChamber(const DriftChamber& that);
    DriftChamber( const string& host,
                  const string& user,
                  const string& db,
                  const bool& quiet = false,
                  const bool& verbose = false );
    DriftChamber& operator=(const DriftChamber& that);

    // inline members
    const vector<unique_ptr<DCSector>>& sectors() const;
    const DCSector& sector(const size_t& sec) const;

    // members in cpp file
    void fetch_nominal_parameters( const string& host,
                                   const string& user,
                                   const string& db );

  private:
    /// \brief the sectors of the DC
    vector<unique_ptr<DCSector>> _sectors;

};

/**
 * \brief Get a vector of the sectors in the drift chamber
 * \return const reference to DriftChamber::_sectors
 **/
inline
const vector<unique_ptr<DCSector>>& DriftChamber::sectors() const
{
    return _sectors;
}

/**
 * \brief Get a sector in the DC
 * \param [in] sec The sector index within the drift chamber (counting from zero)
 * \return const reference to DriftChamber::_sectors[sec]
 **/
inline
const DCSector& DriftChamber::sector(const size_t& sec) const
{
    return *_sectors[sec];
}

} /* namespace clas12::geometry */
} /* namespace clas12 */

#endif /* __CLAS12_GEOMETRY_DRIFT_CHAMBER_HPP__ */
