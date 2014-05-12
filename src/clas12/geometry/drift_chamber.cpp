#ifdef DEBUG
#include <iostream>
using std::clog;
using std::endl;
#endif

#include <cstddef>
#include <numeric>
#include <string>
#include <vector>

#include "clas12/ccdb/constants_table.hpp"

#include "drift_chamber.hpp"

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
DriftChamber::DriftChamber()
{
}

/**
 * \brief copy constructor
 **/
DriftChamber::DriftChamber(const DriftChamber& that)
{
    for (int i=0; i<that._sectors.size(); i++)
    {
        const DCSector& sector = *that._sectors[i];
        _sectors.emplace_back(new DCSector(sector,this,i));
    }
}

/**
 * \brief assigment operator
 **/
DriftChamber& DriftChamber::operator=(const DriftChamber& that)
{
    // copy is very expensive, so we test for self-assignment
    if (this != &that)
    {
        // do the copy
        _sectors.clear();
        for (int i=0; i<that._sectors.size(); i++)
        {
            const DCSector& sector = *that._sectors[i];
            _sectors.emplace_back(new DCSector(sector,this,i));
        }
    }
    return *this;
}

/**
 * \brief constructor which fetches the nominal geometry from the database
 *
 * This calls DriftChamber::fetch_nominal_parameters(dataprovider)
 **/
DriftChamber::DriftChamber( Calibration* calib,
                            const bool& quiet /*= false*/,
                            const bool& verbose /*= false*/ )
{
    fetch_nominal_parameters(calib);
}

/**
 * \brief fills the DriftChamber class with the nominal geometry
 *
 * The nominal geometry is identical for each sector and therefore
 * after this call, there is much redundancy in the geometry. The
 * parameters obtained are the so-called "core" paramters and
 * it is expected that additional alignment paramters will be
 * obtained from the database in a later method-call.
 *
 * \param [in] dataprovider the ccdb::DataProvider object
 **/
void DriftChamber::fetch_nominal_parameters(Calibration* calib)
{
    #ifdef DEBUG
    clog << "DriftChamber::fetch_nominal_parameters()...\n";
    #endif
    static const double deg2rad = 3.14159265358979 / 180.;
    using namespace drift_chamber;

    // here we connect to the CCDB (MySQL) databse and request
    // the nominal geometry parameters for the Drift Chamber.
    // These numbers come from four tables: dc, region, superlayer,
    // and layer.

    #ifdef DEBUG
    clog << "dc...\n";
    #endif
    ConstantsTable table_dc(calib,"/geometry/dc/dc");
    size_t nsectors = table_dc.elem<size_t>("nsectors"); // n
    size_t nregions = table_dc.elem<size_t>("nregions"); // n

    #ifdef DEBUG
    clog << "nsectors: " << nsectors << endl;
    clog << "nregions: " << nregions << endl;
    #endif

    #ifdef DEBUG
    clog << "region...\n";
    #endif
    ConstantsTable table_r(calib,"/geometry/dc/region");
    vector<size_t> nsuperlayers = table_r.col<size_t>("nsuperlayers"); // n
    vector<double> dist2tgt = table_r.col<double>("dist2tgt"); // cm
    vector<double> frontgap = table_r.col<double>("frontgap"); // cm
    vector<double> midgap   = table_r.col<double>("midgap"); // cm
    vector<double> backgap  = table_r.col<double>("backgap"); // cm
    vector<double> thopen   = table_r.col<double>("thopen"); // deg
    vector<double> thtilt   = table_r.col<double>("thtilt"); // deg
    vector<double> xdist    = table_r.col<double>("xdist"); // cm

    #ifdef DEBUG
    clog << "superlayer...";
    #endif
    ConstantsTable table_sl(calib,"/geometry/dc/superlayer");
    vector<size_t> nsenselayers  = table_sl.col<size_t>("nsenselayers"); // n
    vector<size_t> nguardlayers  = table_sl.col<size_t>("nguardlayers"); // n
    vector<size_t> nfieldlayers  = table_sl.col<size_t>("nfieldlayers"); // n
    vector<double> thster        = table_sl.col<double>("thster"); // deg
    vector<double> thmin         = table_sl.col<double>("thmin"); // deg
    vector<double> wpdist        = table_sl.col<double>("wpdist"); // cm
    vector<double> cellthickness = table_sl.col<double>("cellthickness"); // n(wpdist)

    #ifdef DEBUG
    clog << "layer...";
    #endif
    ConstantsTable table_l(calib,"/geometry/dc/layer");
    size_t nsensewires = table_l.elem<size_t>("nsensewires"); // n
    size_t nguardwires = table_l.elem<size_t>("nguardwires"); // n


    // Now we fill the sectors object which holds all these
    // core parameters. Here, many numbers will be redundant.
    // It is expected that this will change once efficiency
    // alignment and other calibrations are taken into effect.
    _sectors.clear();

    for (size_t sec=0; sec<nsectors; sec++)
    {
        _sectors.emplace_back(new DCSector(this,sec));
        DCSector& sector = *_sectors[sec];

        sector._regions.clear();

        for (size_t reg=0; reg<nregions; reg++)
        {
            sector._regions.emplace_back(new Region(&sector,reg));
            Region& region = *sector._regions[reg];

            region._superlayers.clear();

            region._dist2tgt = dist2tgt[reg];
            region._frontgap = frontgap[reg];
            region._midgap   = midgap[reg];
            region._backgap  = backgap[reg];
            region._thopen   = thopen[reg] * deg2rad;
            region._thtilt   = thtilt[reg] * deg2rad;
            region._xdist    = xdist[reg];

            for (size_t slyr=0; slyr<nsuperlayers[reg]; slyr++)
            {

                // nslyr is the "global" superlayer number starting
                // from 0 and going to 5
                int nslyr = slyr;
                for (int n=0; n<reg; n++)
                {
                    nslyr += nsuperlayers[n];
                }

                region._superlayers.emplace_back(new Superlayer(&region,slyr));
                Superlayer& superlayer = *region._superlayers[slyr];

                superlayer._senselayers.clear();
                superlayer._guardlayers.clear();

                superlayer._nfieldlayers = nfieldlayers[nslyr];
                superlayer._thster = thster[nslyr] * deg2rad;
                superlayer._thmin  = thmin[nslyr] * deg2rad;
                superlayer._wpdist = wpdist[nslyr];
                superlayer._cellthickness = cellthickness[nslyr];

                for (size_t lyr=0; lyr<nsenselayers[slyr]; lyr++)
                {
                    superlayer._senselayers.emplace_back(new Senselayer(&superlayer,lyr));
                    Senselayer& layer = *superlayer._senselayers[lyr];

                    layer._sensewires.assign(nsensewires,true);
                    layer._nguardwires = nguardwires;
                }

                for (size_t lyr=0; lyr<nguardlayers[slyr]; lyr++)
                {
                    superlayer._guardlayers.emplace_back(new Guardlayer(&superlayer,lyr));
                    Guardlayer& layer = *superlayer._guardlayers[lyr];

                    layer._nwires = nsensewires + nguardwires;
                }
            }
        }
    }

    #ifdef DEBUG
    clog << "done fetching numbers from database for DC.\n";
    #endif
}

} /* namespace clas12::geometry */
} /* namespace clas12 */
