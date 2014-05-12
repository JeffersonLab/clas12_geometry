#include <iostream>
using std::clog;
using std::endl;

#include <cstddef>
#include <numeric>
#include <string>
#include <vector>

#include "clas12/ccdb/constants_table.hpp"

#include "central_tracker.hpp"

namespace clas12
{
namespace geometry
{

using std::string;
using std::vector;

/**
 * \brief default constructor
 **/
CentralTracker::CentralTracker()
: _bst(this)
//, _fst(this)
{
}

/**
 * \brief copy constructor
 **/
CentralTracker::CentralTracker(const CentralTracker& that)
: _bst(that._bst, this)
//, _fst(that._fst, this)
{
}

/**
 * \brief assigment operator
 **/
CentralTracker& CentralTracker::operator=(const CentralTracker& that)
{
    // Same object?
    if (this != &that)
    {
        // No, so we do the copy
        _bst = that._bst;
        _bst._svt = this;
        //_fst = that._fst;
        //_fst._svt = this;
    }
    return *this;
}

/**
 * \brief constructor which fetches the nominal geometry from the database
 *
 * This calls CentralTracker::fetch_nominal_parameters(host,user,db)
 **/
CentralTracker::CentralTracker(Calibration* calib)
: _bst(this)
//, _fst(this)
{
    fetch_nominal_parameters(calib);
}

/**
 * \brief fills the CentralTracker class with the nominal geometry
 *
 * \param [in] host the calibration constants host name. typically:
 * clasdb.jlab.org
 * \param [in] user the database user name. typically: clasuser
 * \param [in] db the database name. typically: clas12
 **/
void CentralTracker::fetch_nominal_parameters(Calibration* calib)
{
    _bst.fetch_nominal_parameters(calib);
}

} /* namespace clas12::geometry */
} /* namespace clas12 */
