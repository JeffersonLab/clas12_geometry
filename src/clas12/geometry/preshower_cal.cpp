#include <iostream>
using std::clog;
using std::endl;

#include <cstddef>
#include <numeric>
#include <string>
#include <vector>

#include "geometry/euclid_vector.hpp"
#include "clas12/ccdb/constants_table.hpp"

#include "preshower_cal.hpp"

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
PreshowerCal::PreshowerCal()
{}

/**
 * \brief copy constructor
 **/
PreshowerCal::PreshowerCal(const PreshowerCal& that)
{
    for (int i=0; i<that._sectors.size(); i++)
    {
        const PCalSector& sector = *that._sectors[i];
        _sectors.emplace_back(new PCalSector(sector,this,i));
    }
}

/**
 * \brief assigment operator
 **/
PreshowerCal& PreshowerCal::operator=(const PreshowerCal& that)
{
    // copy is very expensive, so we test for self-assignment
    if (this != &that)
    {
        // do the copy
        _sectors.clear();
        for (int i=0; i<that._sectors.size(); i++)
        {
            const PCalSector& sector = *that._sectors[i];
            _sectors.emplace_back(new PCalSector(sector,this,i));
        }
    }
    return *this;
}

/**
 * \brief constructor which fetches the nominal geometry from the database
 *
 * This calls PreshowerCal::fetch_nominal_parameters(host,user,db)
 **/
PreshowerCal::PreshowerCal( Calibration* calib,
                        const bool& quiet /*= false*/,
                        const bool& verbose /*= false*/ )
{
    fetch_nominal_parameters(calib);
}

/**
 * \brief fills the PreshowerCal class with the nominal geometry
 *
 * The nominal geometry is identical for each sector and therefore
 * after this call, there is much redundancy in the geometry. The
 * parameters obtained are the so-called "core" parameters and
 * it is expected that additional alignment parameters will be
 * obtained from the database in a later method-call.
 *
 * \param [in] host the calibration constants host name. typically:
 * clasdb.jlab.org
 * \param [in] user the database user name. typically: clasuser
 * \param [in] db the database name. typically: clas12
 **/
void PreshowerCal::fetch_nominal_parameters(Calibration* calib)
{
    #ifdef DEBUG
    clog << "PreshowerCal::fetch_nominal_parameters()...\n";
    #endif
    static const double deg2rad = 3.14159265358979 / 180.;

    using namespace preshower_cal;

}

} // namespace clas12::geometry
} // namespace clas12
