#include <cstddef>
#include <string>
#include <vector>

#include "clas12/ccdb/constants_table.hpp"

#include "clas12/geometry/central_tracker.hpp"

namespace clas12
{
namespace geometry
{
namespace central_tracker
{

using ::clas12::geometry::CentralTracker;
using namespace barrel_svt;

/**
 * \brief constructor.
 *
 * This forces the BST to have a parent SVT object pointer.
 *
 * \param [in] svt a pointer to the parent CentralTracker class
 **/
BarrelSVT::BarrelSVT(const CentralTracker* svt)
: _svt(svt)
{
}

/**
 * \brief copy constructor.
 *
 * This forces the BST to have a parent SVT object pointer.
 *
 * \param [in] that the BarrelSVT being copied
 * \param [in] svt a pointer to the parent CentralTracker class
 **/
BarrelSVT::BarrelSVT(const BarrelSVT& that, const CentralTracker* svt)
: _svt(svt)
{
    for (size_t i=0; i<that._regions.size(); i++)
    {
        const Region& region = *that._regions[i];
        _regions.emplace_back(new Region(region,this,i));
    }
}

/**
 * \brief assignment operator
 *
 * This is needed by the top-level SVT class to handle
 * copying of the SVT geometry object.
 *
 * \param [in] that the BarrelSVT being copied
 *
 * \return the new BarrelSVT object
 **/
BarrelSVT& BarrelSVT::operator=(const BarrelSVT& that)
{
    _svt = that._svt;
    for (size_t i=0; i<that._regions.size(); i++)
    {
        const Region& region = *that._regions[i];
        _regions.emplace_back(new Region(region,this,i));
    }
    return *this;
}


/**
 * \brief fills the Barrel SVT class with the nominal geometry
 *
 * \param [in] host the calibration constants host name. typically:
 * clasdb.jlab.org
 * \param [in] user the database user name. typically: clasuser
 * \param [in] db the database name. typically: clas12
 **/
void BarrelSVT::fetch_nominal_parameters( const string& host,
                                          const string& user,
                                          const string& db )
{
    static const double deg2rad = 3.14159265358979 / 180.;

    // here we connect to the CCDB (MySQL) databse and request
    // the nominal geometry parameters for the Barrel Vertex Tracker.
    // These numbers come from two tables: region and sector
    clas12::ccdb::ConstantsTable table;
    string conn_str = table.connection_string(user,host,"3306",db);

    #ifdef DEBUG
    clog << "bst...";
    #endif
    table.load_constants("/geometry/bst/bst", conn_str);
    size_t nregions      = table.elem<size_t>("nregions"); // n
    double readoutPitch  = table.elem<double>("readoutPitch"); // mm(?)
    double siliconWidth  = table.elem<double>("siliconWidth"); // mm(?)

    #ifdef DEBUG
    clog << "region...";
    #endif
    table.load_constants("/geometry/bst/region", conn_str);
    vector<bool>   status    = table.col<bool  >("status");     // bool
    vector<size_t> nsectors  = table.col<size_t>("nsectors");   // n
    vector<size_t> nlayers   = table.col<size_t>("nlayers");    // n
    vector<double> radius    = table.col<double>("radius");     // mm
    vector<double> zstart    = table.col<double>("zstart");     // mm
    vector<double> phi       = table.col<double>("theta");        // deg
    vector<double> layergap  = table.col<double>("layergap");   // mm


    #ifdef DEBUG
    clog << "sector...";
    #endif
    table.load_constants("/geometry/bst/sector", conn_str);
    size_t nstrips        = table.elem<size_t>("nstrips");       // n
    double physSenLen     = table.elem<double>("physSenLen");    // mm
    double physSenWid     = table.elem<double>("physSenWid");    // mm
    double activeSenLen   = table.elem<double>("activeSenLen");  // mm
    double activeSenWid   = table.elem<double>("activeSenWid");  // mm
    double deadZnSenLen1  = table.elem<double>("deadZnSenLen1"); // mm
    double deadZnSenLen2  = table.elem<double>("deadZnSenLen2"); // mm
    double deadZnSenLen3  = table.elem<double>("deadZnSenLen3"); // mm
    double deadZnSenWid   = table.elem<double>("deadZnSenWid");  // mm
    double fillerthick    = table.elem<double>("fillerthick");   // mm
    double startAngle     = table.elem<double>("startAngle");    // deg
    double endAngle       = table.elem<double>("endAngle");      // deg

    // Now we fill the "bst" object which holds all these
    // core parameters. Here, many numbers will be redundant.
    // It is expected that this will change once efficiency
    // alignment and other calibrations are taken into effect.

    _regions.clear();

    for (size_t reg=0; reg<nregions; reg++)
    {
        if (status[reg])
        {
            _regions.emplace_back(new Region(this,reg));
            Region& region = *_regions[reg];

            region._sectors.clear();

            region._radius = radius[reg];
            region._zstart = zstart[reg];
            region._phi    = phi[reg] * deg2rad;

            for (size_t sec=0; sec<nsectors[reg]; sec++)
            {
                region._sectors.emplace_back(new Sector(&region,sec));
                Sector& sector = *region._sectors[sec];

                sector._layergap      = layergap[reg];
                sector._fillerthick   = fillerthick;

                for (size_t lyr=0; lyr<nlayers[reg]; lyr++)
                {
                    sector._layers.emplace_back(new Layer(&sector,lyr));
                    Layer& layer = *sector._layers[lyr];

                    layer._strips.assign(nstrips,true);
                    layer._readoutPitch  = readoutPitch;
                    layer._siliconWidth  = siliconWidth;
                    layer._physSenLen    = physSenLen;
                    layer._physSenWid    = physSenWid;
                    layer._activeSenLen  = activeSenLen;
                    layer._activeSenWid  = activeSenWid;
                    layer._deadZnSenLen1 = deadZnSenLen1;
                    layer._deadZnSenLen2 = deadZnSenLen2;
                    layer._deadZnSenLen3 = deadZnSenLen3;
                }
            }
        }
    }
}

} /* namespace clas12::geometry::central_tracker */
} /* namespace clas12::geometry */
} /* namespace clas12 */

