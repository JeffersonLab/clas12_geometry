#include <iostream>
using std::clog;
using std::endl;

#include <cstddef>
#include <numeric>
#include <string>
#include <vector>

#include "geometry/euclid_vector.hpp"
#include "clas12/ccdb/constants_table.hpp"

#include "forward_tof.hpp"

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
ForwardTOF::ForwardTOF()
{}

/**
 * \brief copy constructor
 **/
ForwardTOF::ForwardTOF(const ForwardTOF& that)
{
    for (int i=0; i<that._sectors.size(); i++)
    {
        const FTOFSector& sector = *that._sectors[i];
        _sectors.emplace_back(new FTOFSector(sector,this,i));
    }
}

/**
 * \brief assigment operator
 **/
ForwardTOF& ForwardTOF::operator=(const ForwardTOF& that)
{
    // copy is very expensive, so we test for self-assignment
    if (this != &that)
    {
        // do the copy
        _sectors.clear();
        for (int i=0; i<that._sectors.size(); i++)
        {
            const FTOFSector& sector = *that._sectors[i];
            _sectors.emplace_back(new FTOFSector(sector,this,i));
        }
    }
    return *this;
}

/**
 * \brief constructor which fetches the nominal geometry from the database
 *
 * This calls ForwardTOF::fetch_nominal_parameters(host,user,db)
 **/
ForwardTOF::ForwardTOF( Calibration* calib,
                        const bool& quiet /*= false*/,
                        const bool& verbose /*= false*/ )
{
    fetch_nominal_parameters(calib);
}

/**
 * \brief fills the ForwardTOF class with the nominal geometry
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
void ForwardTOF::fetch_nominal_parameters(Calibration* calib)
{
    #ifdef DEBUG
    clog << "ForwardTOF::fetch_nominal_parameters()...\n";
    #endif
    static const double deg2rad = 3.14159265358979 / 180.;

    using namespace forward_tof;

    // the nominal geometry parameters for the FTOF.
    // These numbers come from six tables:
    ConstantsTable table_ftof(calib,"/geometry/ftof/ftof");
    ConstantsTable table_p1a(calib,"/geometry/ftof/panel1a/panel");
    ConstantsTable table_p1b(calib,"/geometry/ftof/panel1b/panel");
    ConstantsTable table_p2(calib,"/geometry/ftof/panel2/panel");
    ConstantsTable table_p1a_pad(calib,"/geometry/ftof/panel1a/paddles");
    ConstantsTable table_p1b_pad(calib,"/geometry/ftof/panel1b/paddles");
    ConstantsTable table_p2_pad(calib,"/geometry/ftof/panel2/paddles");

    #ifdef DEBUG
    clog << "ftof...\n";
    #endif
    size_t nsectors = table_ftof.elem<size_t>("nsectors"); // n
    size_t npanels  = table_ftof.elem<size_t>("npanels"); // n

    #ifdef DEBUG
    clog << "nsectors: " << nsectors << endl;
    clog << "npanels: " <<  npanels << endl;
    #endif

    #ifdef DEBUG
    clog << "panels...\n";
    #endif

    vector<double> paddle_width = {
            table_p1a.elem<double>("paddlewidth"), // cm
            table_p1b.elem<double>("paddlewidth"), // cm
            table_p2.elem<double>("paddlewidth") };// cm

    vector<double> paddle_thick = {
            table_p1a.elem<double>("paddlethickness"), // cm
            table_p1b.elem<double>("paddlethickness"), // cm
            table_p2.elem<double>("paddlethickness") };// cm

    vector<double> panel_thtilt = {
            table_p1a.elem<double>("thtilt"), // deg
            table_p1b.elem<double>("thtilt"), // deg
            table_p2.elem<double>("thtilt") };// deg

    vector<double> panel_thmin = {
            table_p1a.elem<double>("thmin"), // deg
            table_p1b.elem<double>("thmin"), // deg
            table_p2.elem<double>("thmin") };// deg

    vector<double> panel_dist2edge = {
            table_p1a.elem<double>("dist2edge"), // cm
            table_p1b.elem<double>("dist2edge"), // cm
            table_p2.elem<double>("dist2edge") };// cm

    vector<double> paddle_gap = {
            table_p1a.elem<double>("gap"), // cm
            table_p1b.elem<double>("gap"), // cm
            table_p2.elem<double>("gap") };// cm

    double panel1b_pairgap = table_p1b.elem<double>("pairgap"); // cm

    vector<double> wrapper_thick = {
            table_p1a.elem<double>("wrapperthickness"), // cm
            table_p1b.elem<double>("wrapperthickness"), // cm
            table_p2.elem<double>("wrapperthickness") };// cm

    #ifdef DEBUG
    clog << "paddles...\n";
    #endif
    vector<vector<double>> paddle_meas_lengths = {
            table_p1a_pad.col<double>("Length"), // cm
            table_p1b_pad.col<double>("Length"), // cm
            table_p2_pad.col<double>("Length") };// cm

   vector<vector<double>> paddle_slopes = {
            table_p1a_pad.col<double>("Slope"),
            table_p1b_pad.col<double>("Slope"),
            table_p2_pad.col<double>("Slope") };

   vector<vector<double>> paddle_interXs = {
            table_p1a_pad.col<double>("Intercept"),
            table_p1b_pad.col<double>("Intercept"),
            table_p2_pad.col<double>("Intercept") };

    // Now we fill the sectors object which holds all these
    // core parameters. Here, many numbers will be redundant.
    // It is expected that this will change once efficiency
    // alignment and other calibrations are taken into effect.
    _sectors.clear();

    for (size_t sec=0; sec<nsectors; sec++)
    {
        _sectors.emplace_back(new FTOFSector(this,sec));
        FTOFSector& sector = *_sectors[sec];

        sector._panels.clear();

        for (size_t pan=0; pan<npanels; pan++)
        {
            sector._panels.emplace_back(new Panel(&sector,pan));
            Panel& panel = *sector._panels[pan];

            panel._paddles.clear();

            panel._paddle_width      = paddle_width[pan] ;
            panel._paddle_thickness  = paddle_thick[pan] ;
            panel._thtilt            = panel_thtilt[pan] * deg2rad;
            panel._thmin             = panel_thmin[pan] * deg2rad;
            panel._dist2edge         = panel_dist2edge[pan] ;
            panel._paddle_gap        = paddle_gap[pan] ;
            if (pan == 1)
            {
                panel._paddle_pairgap = panel1b_pairgap;
            }
            else
            {
                panel._paddle_pairgap = 0.;
            }
            panel._wrapper_thickness = wrapper_thick[pan] ;

            panel._paddle_meas_lengths.resize(paddle_meas_lengths[pan].size());
            panel._paddle_slopes.resize(paddle_slopes[pan].size());
            panel._paddle_interXs.resize(paddle_interXs[pan].size());

            for (int i=0; i<paddle_meas_lengths[pan].size(); i++)
            {
                panel._paddle_slopes[i]   = paddle_slopes[pan][i];
                panel._paddle_interXs[i]  = paddle_interXs[pan][i];

            }

            panel._paddles.assign(paddle_meas_lengths[pan].size(),true);
            panel._paddles.assign(paddle_slopes[pan].size(),true);
            panel._paddles.assign(paddle_interXs[pan].size(),true);
        }
    }

    #ifdef DEBUG
    clog << "done fetching numbers from database for FTOF.\n";
    #endif
}

} // namespace clas12::geometry
} // namespace clas12
