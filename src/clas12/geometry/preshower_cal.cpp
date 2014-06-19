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

#include "clas12/log.hpp"

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
    LOG(debug) << "PreshowerCal::fetch_nominal_parameters()...";
    static const double deg2rad = 3.14159265358979 / 180.;

    using namespace preshower_cal;

// the nominal geometry parameters for the PCAL.
// These numbers come from three tables:
    ConstantsTable table_pcal(calib,"/geometry/pcal/pcal");
    ConstantsTable table_view_u(calib,"/geometry/pcal/Uview");
    ConstantsTable table_view_v(calib,"/geometry/pcal/Vview");
    ConstantsTable table_view_w(calib,"/geometry/pcal/Wview");

    LOG(debug) << "pcal...";
    size_t nsectors      = table_pcal.elem<size_t>("nsectors"); // n
    size_t nviews        = table_pcal.elem<size_t>("nviews"); // n
    size_t nlayers       = table_pcal.elem<size_t>("nlayers"); // n
    size_t nsteel        = table_pcal.elem<size_t>("nsteel"); // n
    size_t nfoam         = table_pcal.elem<size_t>("nfoam"); // n
    double view_angle    = table_pcal.elem<double>("view_angle");//n
    double thtilt        = table_pcal.elem<double>("thtilt");//n
    double wrapper_thick = table_pcal.elem<double>("wrapper_thick");//cm
    double strip_thick   = table_pcal.elem<double>("strip_thick");//cm
    double steel_thick   = table_pcal.elem<double>("steel_thick");//cm
    double foam_thick    = table_pcal.elem<double>("foam_thick");//cm
    double lead_thick    = table_pcal.elem<double>("lead_thick");//cm
    double strip_width   = table_pcal.elem<double>("strip_width");//cm
    double dist2tgt      = table_pcal.elem<double>("dist2tgt");//cm
    double yhigh         = table_pcal.elem<double>("yhigh");//cm

    LOG(debug) << "nsectors: "      << nsectors << endl
               << "nviews: "        << nviews << endl
               << "nlayers: "       << nlayers << endl
               << "nsteel: "        << nsteel << endl
               << "nfoam: "         << nfoam << endl
               << "view_angle: "    << view_angle << endl
               << "thtilt: "        << thtilt << endl
               << "wrapper_thick: " << wrapper_thick<< endl
               << "strip_thick: "   << strip_thick<< endl
               << "steel_thick: "   << steel_thick<< endl
               << "foam_thick: "    << foam_thick<< endl
               << "lead_thick: "    << lead_thick<< endl
               << "strip_width: "   << strip_width << endl
               << "dist2tgt: "      << dist2tgt<< endl
               << "yhigh: "         << yhigh<< endl;

    LOG(debug) << "widths...";



    double u_nstrips    =  table_view_u.elem<double>("nstrips");
    double u_max_length =  table_view_u.elem<double>("max_length");

    double v_nstrips    =  table_view_v.elem<double>("nstrips");
    double v_max_length =  table_view_v.elem<double>("max_length");

    double w_nstrips    =  table_view_w.elem<double>("nstrips");
    double w_max_length =  table_view_w.elem<double>("max_length");





    // Now we fill the sectors object which holds all these
    // core parameters. Here, many numbers will be redundant.
    // It is expected that this will change once efficiency
    // alignment and other calibrations are taken into effect.
    _sectors.clear();

    for (size_t sec=0; sec<nsectors; sec++)
    {
        _sectors.emplace_back(new PCalSector(this,sec));
        PCalSector& sector = *_sectors[sec];
        sector._layers.clear();

        sector._nlayers     = nlayers;
        sector._nfoam       = nfoam;
        sector._nsteel      = nsteel;
        sector._foam_thick  = foam_thick*0.1;
        sector._steel_thick = steel_thick*0.1;
        sector._thtilt      = thtilt*deg2rad ;
        sector._dist2tgt    = dist2tgt*0.1;
        sector._yhigh       = yhigh*0.1;

        for (size_t lyr=0; lyr<nlayers; lyr++)
        {
            sector._layers.emplace_back(new Layer(&sector,lyr));
            Layer& layer = *sector._layers[lyr];

            layer._views.clear();
            layer._nviews          = nviews;
            layer._view_angle      = view_angle*deg2rad ;
            layer._wrapper_thick   = wrapper_thick*0.1;
            layer._strip_thick     = strip_thick*0.1;
            layer._lead_thick      = lead_thick*0.1;
            layer._strip_width     = strip_width*0.1;

            for (size_t iview=0; iview<nviews; iview++)
            {
                layer._views.emplace_back(new View(&layer,iview));
                View& view = *layer._views[iview];

                view._strips.clear();

                   if (view.name() == "u")
                   {
                       view._nstrips    = u_nstrips;
                       view._max_length = u_max_length*0.1;

                       view._strips.assign(view.nstrips(),true);
                    }
                    else if (view.name() == "v")
                    {
                       view._nstrips    = v_nstrips;
                       view._max_length = v_max_length*0.1;

                       view._strips.assign(view.nstrips(),true);
                    }
                    else if (view.name() == "w")
                    {
                       view._nstrips    = w_nstrips;
                       view._max_length = w_max_length*0.1;

                       view._strips.assign(view.nstrips(),true);
                    }


            }

        }
    }

    LOG(debug) << "done fetching numbers from database for PCAL.";
}




} // namespace clas12::geometry
} // namespace clas12
