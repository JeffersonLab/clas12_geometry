#include <cstddef>
#include <numeric>
#include <string>
#include <vector>

#include "clas12/ccdb/constants_table.hpp"

#include "high_threshold_cerenkov.hpp"

namespace clas12
{
namespace geometry
{

using std::string;
using std::vector;

using clas12::ccdb::ConstantsTable;

/**
 * \brief default constructor
 **/
HighThresholdCerenkov::HighThresholdCerenkov()
{
}

/**
 * \brief copy constructor
 **/
HighThresholdCerenkov::HighThresholdCerenkov(const HighThresholdCerenkov& that)
{
    for (int i=0; i<that._sectors.size(); i++)
    {
        const HTCCSector& sector = *that._sectors[i];
        _sectors.emplace_back(new HTCCSector(sector,this,i));
    }
}

/**
 * \brief assigment operator
 **/
HighThresholdCerenkov& HighThresholdCerenkov::operator=(const HighThresholdCerenkov& that)
{
    // copy is very expensive, so we test for self-assignment
    if (this != &that)
    {
        // do the copy
        _sectors.clear();
        for (int i=0; i<that._sectors.size(); i++)
        {
            const HTCCSector& sector = *that._sectors[i];
            _sectors.emplace_back(new HTCCSector(sector,this,i));
        }
    }
    return *this;
}

/**
 * \brief constructor which fetches the nominal geometry from the database
 *
 * This calls HighThresholdCerenkov::fetch_nominal_parameters(dataprovider)
 **/
HighThresholdCerenkov::HighThresholdCerenkov( Calibration* calib,
                                              bool quiet,
                                              bool verbose )
{
    fetch_nominal_parameters(calib);
}

/**
 * \brief fills the HighThresholdCerenkov class with the nominal geometry
 *
 * The nominal geometry is identical for each sector and therefore
 * after this call, there is much redundancy in the geometry. The
 * parameters obtained are the so-called "core" paramters and
 * it is expected that additional alignment paramters will be
 * obtained from the database in a later method-call.
 *
 * \param [in] dataprovider the ccdb::DataProvider object
 **/
void HighThresholdCerenkov::fetch_nominal_parameters(Calibration* calib)
{
    static const double deg2rad = 3.14159265358979 / 180.;
    using namespace high_threshold_cerenkov;
}

} // namespace clas12::geometry
} // namespace clas12
