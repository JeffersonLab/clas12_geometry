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
    ConstantsTable table_view_u(calib,"/geometry/pcal/view_u");
    ConstantsTable table_view_vw(calib,"/geometry/pcal/view_vw");

    LOG(debug) << "pcal...";
    size_t nsectors      = table_pcal.elem<size_t>("nsectors"); // n
    size_t nviews        = table_pcal.elem<size_t>("nviews"); // n
    size_t nlayers       = table_pcal.elem<size_t>("nlayers"); // n
    size_t nsteel        = table_pcal.elem<size_t>("nsteel"); // n
    size_t nfoam         = table_pcal.elem<size_t>("nfoam"); // n
    double view_angle    = table_pcal.elem<double>("view_angle");//n
    double thtilt        = table_pcal.elem<double>("thtilt");//n
    double wrapper_thick = table_pcal.elem<double>("wrapper_thick");//cm
    double scint_thick   = table_pcal.elem<double>("scint_thick");//cm
    double steel_thick   = table_pcal.elem<double>("steel_thick");//cm
    double foam_thick    = table_pcal.elem<double>("foam_thick");//cm
    double lead_thick    = table_pcal.elem<double>("lead_thick");//cm
    double max_u_length  = table_pcal.elem<double>("max_u_length");//cm
    double max_vw_length = table_pcal.elem<double>("max_vw_length");//cm

    LOG(debug) << "nsectors: "      << nsectors << endl
               << "nviews: "        << nviews << endl
               << "nlayers: "       << nlayers << endl
               << "nsteel: "        << nsteel << endl
               << "nfoam: "         << nfoam << endl
               << "view_angle: "    << view_angle << endl
               << "thtilt: "        << thtilt << endl
               << "wrapper_thick: " << wrapper_thick
               << "scint_thick: "   << scint_thick
               << "steel_thick: "   << steel_thick
               << "foam_thick: "    << foam_thick
               << "lead_thick: "    << lead_thick
               << "max_u_length: "  << max_u_length
               << "max_vw_length: " << max_vw_length;

    LOG(debug) << "widths...";



    vector<double> scint_width_u =  table_view_u.col<double>("width");
    vector<double> scint_width_vw = table_view_vw.col<double>("width");

    /*vector<vector<double>> scint_width =
    {
        table_view_u.col<double>("width"),
        table_view_vw.col<double>("width") //mm
    };
    */


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

        for (size_t lyr=0; lyr<nlayers; lyr++)
        {
            sector._layers.emplace_back(new Layer(&sector,lyr));
            Layer& layer = *sector._layers[lyr];

            layer._views.clear();
            layer._nviews          = nviews;
            layer._view_angle      = view_angle*deg2rad ;
            layer._wrapper_thick   = wrapper_thick*0.1;
            layer._scint_thick     = scint_thick*0.1;
            layer._lead_thick      = lead_thick*0.1;


            for (size_t iview=0; iview<nviews; iview++)
            {
                layer._views.emplace_back(new View(&layer,iview));
                View& view = *layer._views[iview];

                view._strips.clear();

                   if (view.name() == "u")
                   {
                        view._scint_max_length = max_u_length*0.1;
                        view._scint_width.resize(scint_width_u.size());

                        for (int i = 0; i<scint_width_u.size(); i++)
                        {
                           view._scint_width[i] = scint_width_u[i]*0.1;

                           LOG(debug) <<"layer = "<<lyr<<"  view = "<< iview <<"  "<<
                           i<<"  "<< view._scint_width[i]<<endl;

                        }
                        view._strips.assign(scint_width_u.size(),true);
                    }
                    else if (view.name() == "v" || view.name() == "w")
                    {
                        view._scint_max_length = max_vw_length*0.1;
                        view._scint_width.resize(scint_width_vw.size());

                        for (int i = 0; i<scint_width_vw.size(); i++)
                        {
                           view._scint_width[i] = scint_width_vw[i]*0.1;

                           LOG(debug) <<"layer = "<<lyr<<"  view = "<< iview<<"  "<<
                            i<<"  "<< view._scint_width[i]<<endl;
                        }
                        view._strips.assign(scint_width_vw.size(),true);
                    }



            }

        }
    }

    LOG(debug) << "done fetching numbers from database for PCAL.";
}












} // namespace clas12::geometry
} // namespace clas12
