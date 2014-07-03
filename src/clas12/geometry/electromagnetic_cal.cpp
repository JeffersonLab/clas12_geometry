#include <iostream>
using std::clog;
using std::endl;

#include <cstddef>
#include <numeric>
#include <string>
#include <vector>

#include "geometry/euclid_vector.hpp"
#include "clas12/ccdb/constants_table.hpp"

#include "electromagnetic_cal.hpp"

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
ElectromagneticCal::ElectromagneticCal()
{}

/**
 * \brief copy constructor
 **/
ElectromagneticCal::ElectromagneticCal(const ElectromagneticCal& that)
{
    for (int i=0; i<that._sectors.size(); i++)
    {
        const ECSector& sector = *that._sectors[i];
        _sectors.emplace_back(new ECSector(sector,this,i));
    }
}

/**
 * \brief assigment operator
 **/
ElectromagneticCal& ElectromagneticCal::operator=(const ElectromagneticCal& that)
{
    // copy is very expensive, so we test for self-assignment
    if (this != &that)
    {
        // do the copy
        _sectors.clear();
        for (int i=0; i<that._sectors.size(); i++)
        {
            const ECSector& sector = *that._sectors[i];
            _sectors.emplace_back(new ECSector(sector,this,i));
        }
    }
    return *this;
}

/**
 * \brief constructor which fetches the nominal geometry from the database
 *
 * This calls ElectromagneticCal::fetch_nominal_parameters(host,user,db)
 **/
ElectromagneticCal::ElectromagneticCal( Calibration* calib,
                        const bool& quiet /*= false*/,
                        const bool& verbose /*= false*/ )
{
    fetch_nominal_parameters(calib);
}

/**
 * \brief fills the ElectromagneticCal class with the nominal geometry
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
void ElectromagneticCal::fetch_nominal_parameters(Calibration* calib)
{
    LOG(debug) << "ElectromagneticCal::fetch_nominal_parameters()...";
    static const double deg2rad = 3.14159265358979 / 180.;

    using namespace electromagnetic_cal;

// the nominal geometry parameters for the EC.
// These numbers come from four tables:
    ConstantsTable table_ec(calib,"/geometry/ec/ec");
    ConstantsTable table_view_u(calib,"/geometry/ec/Uview");
    ConstantsTable table_view_v(calib,"/geometry/ec/Vview");
    ConstantsTable table_view_w(calib,"/geometry/ec/Wview");

    LOG(debug) << "ec...";
    size_t nsectors      = table_ec.elem<size_t>("nsectors"); // n
    size_t nviews        = table_ec.elem<size_t>("nviews"); // n
    size_t nlayers       = table_ec.elem<size_t>("nlayers"); // n
    double view_angle    = table_ec.elem<double>("view_angle");//n
    double thtilt        = table_ec.elem<double>("thtilt");//n
    double wrapper_thick = table_ec.elem<double>("wrapper_thick");
    double strip_thick   = table_ec.elem<double>("strip_thick");
    double alum_thick    = table_ec.elem<double>("alum_thick");
    double lead_thick    = table_ec.elem<double>("lead_thick");
    double dist2tgt      = table_ec.elem<double>("dist2tgt");
    double dist2cnt      = table_ec.elem<double>("dist2cnt");
    double a1            = table_ec.elem<double>("a1");
    double a2            = table_ec.elem<double>("a2");
    double a3            = table_ec.elem<double>("a3");
    double d2            = table_ec.elem<double>("d2");
    double d2prime       = table_ec.elem<double>("d2prime");

    LOG(debug) << "nsectors: "      << nsectors << endl
               << "nviews: "        << nviews << endl
               << "nlayers: "       << nlayers << endl
               << "view_angle: "    << view_angle << endl
               << "thtilt: "        << thtilt << endl
               << "wrapper_thick: " << wrapper_thick<< endl
               << "strip_thick: "   << strip_thick<< endl
               << "alum_thick: "    << steel_thick<< endl
               << "lead_thick: "    << lead_thick<< endl
               << "dist2tgt: "      << dist2tgt<< endl
               << "dist2cnt: "      << dist2tgt<< endl
               << "a1: "            << a1<< endl
               << "a2: "            << a2<< endl
               << "a3: "            << a3<< endl
               << "d2: "            << d2<< endl
               << "d2prime: "       << d2prime<< endl;



    LOG(debug) << "widths...";



    double u_nstrips    =  table_view_u.elem<double>("nstrips");
    double u_shift      =  table_view_u.elem<double>("a4");
    double u_halfwidth  =  table_view_u.elem<double>("a5");
    double u_deltahw    =  table_view_u.elem<double>("a6");

    double v_nstrips    =  table_view_v.elem<double>("nstrips");
    double v_halfwidth  =  table_view_v.elem<double>("a5");
    double v_deltahw    =  table_view_v.elem<double>("a6");


    double w_nstrips    =  table_view_w.elem<double>("nstrips");
    double w_halfwidth  =  table_view_w.elem<double>("a5");
    double w_deltahw    =  table_view_w.elem<double>("a6");





    // Now we fill the sectors object which holds all these
    // core parameters. Here, many numbers will be redundant.
    // It is expected that this will change once efficiency
    // alignment and other calibrations are taken into effect.
    _sectors.clear();

    for (size_t sec=0; sec<nsectors; sec++)
    {
        _sectors.emplace_back(new ECSector(this,sec));
        ECSector& sector = *_sectors[sec];
        sector._layers.clear();

        sector._nlayers     = nlayers;
        sector._alum_thick  = alum_thick*0.1;
        sector._thtilt      = thtilt*deg2rad ;
        sector._dist2tgt    = dist2tgt*0.1;
        sector._dist2cnt    = dist2cnt*0.1;
        sector._a1          = a1*0.1;
        sector._a2          = a2*0.1;
        sector._a3          = a3*0.1;
        sector._d2          = d2*0.1;
        sector._d2prime     = d2prime*0.1;

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

            for (size_t iview=0; iview<nviews; iview++)
            {
                layer._views.emplace_back(new View(&layer,iview));
                View& view = *layer._views[iview];

                view._strips.clear();

                   if (view.name() == "u")
                   {
                       view._nstrips    = u_nstrips;
                       view._shift      = u_shift*0.1;
                       view._halfwidth  = u_halfwidth*0.1;
                       view._deltahw    = u_deltahw*0.1;

                       view._strips.assign(view.nstrips(),true);
                    }
                    else if (view.name() == "v")
                    {
                       view._nstrips    = v_nstrips;
                       view._shift      = 0;
                       view._halfwidth  = v_halfwidth*0.1;
                       view._deltahw    = v_deltahw*0.1;

                       view._strips.assign(view.nstrips(),true);
                    }
                    else if (view.name() == "w")
                    {
                       view._nstrips    = w_nstrips;
                       view._shift      = 0;
                       view._halfwidth  = w_halfwidth*0.1;
                       view._deltahw    = w_deltahw*0.1;


                       view._strips.assign(view.nstrips(),true);
                    }


            }

        }
    }

    LOG(debug) << "done fetching numbers from database for EC.";
}




} // namespace clas12::geometry
} // namespace clas12
