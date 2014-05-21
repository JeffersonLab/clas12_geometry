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
    double view_angle    = table_pcal.elem<double>("view_angle");//n
    double wrapper_thick = table_pcal.elem<double>("wrapper_thick");//cm

    LOG(debug) << "nsectors: "      << nsectors << endl
               << "nviews: "        << nviews << endl
               << "nlayers: "       << nlayers << endl
               << "view_angle: "    << view_angle << endl
               << "wrapper_thick: " << wrapper_thick;


    LOG(debug) << "widths...";



    vector<double> strip_width_u =  table_view_u.col<double>("width");
    vector<double> strip_width_vw = table_view_vw.col<double>("width");

    /*vector<vector<double>> strip_width =
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
        sector._views.clear();

        for (size_t iview=0; iview<nviews; iview++)
        {
            sector._views.emplace_back(new View(&sector,iview));
            View& view = *sector._views[iview];


            for (size_t lyr=0; lyr<nlayers; lyr++)
            {
                view._layers.emplace_back(new Layer(&view,lyr));
                Layer& layer = *view._layers[lyr];

                layer._strips.clear();


                //layer._strip_width.resize(strip_width[lyr].size());


                /*for (int i=0; i<layer._strip_width.size(); i++)
                {
                      layer._strip_width[i] = layer._strip_width[i]*0.1; // convert to cm
                }*/

                //layer._strips.assign(strip_width[lyr].size(),true);


                   if (iview == 0)
                   {
                        layer._strip_width.resize(strip_width_u.size());

                        for (int i = 0; i<strip_width_u.size(); i++)
                        {
                           layer._strip_width[i] = strip_width_u[i]*0.1;

                          // LOG(debug) <<"view = "<<iview<<"  layer = "<<lyr<<"  "<<
                          // i<<"  "<< layer._strip_width[i]<<endl;

                        }
                        layer._strips.assign(strip_width_u.size(),true);
                    }
                    else if (iview == 1 || iview == 2)
                    {
                        layer._strip_width.resize(strip_width_vw.size());

                        for (int i = 0; i<strip_width_vw.size(); i++)
                        {
                           layer._strip_width[i] = strip_width_vw[i]*0.1;

                           //LOG(debug) <<"view = "<<iview<<"  layer = "<<lyr<<"  "<<
                            //i<<"  "<< layer._strip_width[i]<<endl;
                        }
                        layer._strips.assign(strip_width_vw.size(),true);
                    }



            }

        }
    }

    LOG(debug) << "done fetching numbers from database for PCAL.";
}












} // namespace clas12::geometry
} // namespace clas12
