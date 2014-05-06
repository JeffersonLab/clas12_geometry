#ifndef CLAS12_GEOMETRY_FORWARD_TOF_HPP
#define CLAS12_GEOMETRY_FORWARD_TOF_HPP

#include <cstddef>
#include <memory>
#include <string>
#include <vector>

#include "CCDB/Providers/DataProvider.h"

#include "forward_tof/sector.hpp"

namespace clas12
{
namespace geometry
{

using std::string;
using std::unique_ptr;
using std::vector;


typedef forward_tof::Sector FTOFSector;

/**
 * \brief The forward time of flight geometry class for CLAS12
 *
 * The FTOF consists of six sectors, each of
 * which with three panels: 1a,1b,2.
 * Each panel consists of a number of scintillator
 * paddles: 23 for 1a, 62 for 1b, 5 for 2.
 **/
class ForwardTOF
{
  public:
    ForwardTOF();
    ForwardTOF(const ForwardTOF& that);
    ForwardTOF( ccdb::DataProvider* dataprovider,
                const bool& quiet = false,
                const bool& verbose = false );
    ForwardTOF& operator=(const ForwardTOF& that);

    // inline members
    const vector<unique_ptr<FTOFSector>>& sectors() const;
    const FTOFSector& sector(const size_t& sec) const;

    // members in cpp file
    void fetch_nominal_parameters(ccdb::DataProvider* dataprovider);

  private:
    /// \brief the sectors of the FTOF
    vector<unique_ptr<FTOFSector>> _sectors;

};

/**
 * \brief Get a vector of the sectors in the FTOF
 * \return const reference to ForwardTOF::_sectors
 **/
inline
const vector<unique_ptr<FTOFSector>>& ForwardTOF::sectors() const
{
    return _sectors;
}

/**
 * \brief Get a sector in the FTOF
 * \param [in] sec The sector index within the FTOF (counting from zero)
 * \return const reference to ForwardTOF::_sectors[sec]
 **/
inline
const FTOFSector& ForwardTOF::sector(const size_t& sec) const
{
    return *_sectors[sec];
}

} // namespace clas12::geometry
} // namespace clas12

#endif // CLAS12_GEOMETRY_FORWARD_TOF_HPP
