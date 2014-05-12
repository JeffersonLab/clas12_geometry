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

using clas12::ccdb::ConstantsTable;

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
void BarrelSVT::fetch_nominal_parameters(Calibration* calib)
{
    static const double deg2rad = 3.14159265358979 / 180.;

    #ifdef DEBUG
    clog << "bst...";
    #endif
    ConstantsTable table_bst(calib,"/geometry/bst/bst");
    size_t nregions      = table_bst.elem<size_t>("nregions"); // n
    double readoutPitch  = table_bst.elem<double>("readoutPitch"); // mm(?)
    double siliconWidth  = table_bst.elem<double>("siliconWidth"); // mm(?)

    #ifdef DEBUG
    clog << "region...";
    #endif
    ConstantsTable table_r(calib,"/geometry/bst/region");
    vector<bool>   status    = table_r.col<bool  >("status");     // bool
    vector<size_t> nsectors  = table_r.col<size_t>("nsectors");   // n
    vector<size_t> nlayers   = table_r.col<size_t>("nlayers");    // n
    vector<double> radius    = table_r.col<double>("radius");     // mm
    vector<double> zstart    = table_r.col<double>("zstart");     // mm
    vector<double> phi       = table_r.col<double>("theta");        // deg
    vector<double> layergap  = table_r.col<double>("layergap");   // mm


    #ifdef DEBUG
    clog << "sector...";
    #endif
    ConstantsTable table_s(calib,"/geometry/bst/sector");
    size_t nstrips        = table_s.elem<size_t>("nstrips");       // n
    double physSenLen     = table_s.elem<double>("physSenLen");    // mm
    double physSenWid     = table_s.elem<double>("physSenWid");    // mm
    double activeSenLen   = table_s.elem<double>("activeSenLen");  // mm
    double activeSenWid   = table_s.elem<double>("activeSenWid");  // mm
    double deadZnSenLen1  = table_s.elem<double>("deadZnSenLen1"); // mm
    double deadZnSenLen2  = table_s.elem<double>("deadZnSenLen2"); // mm
    double deadZnSenLen3  = table_s.elem<double>("deadZnSenLen3"); // mm
    double deadZnSenWid   = table_s.elem<double>("deadZnSenWid");  // mm
    double fillerthick    = table_s.elem<double>("fillerthick");   // mm
    double startAngle     = table_s.elem<double>("startAngle");    // deg
    double endAngle       = table_s.elem<double>("endAngle");      // deg

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

