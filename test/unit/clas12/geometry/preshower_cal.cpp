#define BOOST_TEST_DYN_LINK

#define BOOST_TEST_MODULE clas12_geometry_preshower_cal

#include <boost/test/unit_test.hpp>

#include <iostream>
using std::clog;
using std::endl;

#include <cstddef>
#include <string>
#include <vector>

#include "pugixml.hpp"

#include "geometry/io.hpp"
#include "geometry/line.hpp"

#include "clas12/geometry/preshower_cal.hpp"
//#include "clas12/geometry/output/pcal_volumes.hpp"

#include "clas12/ccdb/constants_table.hpp"

BOOST_AUTO_TEST_SUITE(clas12_geometry_pcal)

BOOST_AUTO_TEST_CASE(constructor)
{
    static const double deg2rad = 3.14159265358979 / 180.;
    //static const double in2cm   = 1./2.54;

    using namespace std;
    using ::geometry::line;
    using ::geometry::plane;
    using namespace clas12::geometry;
    using namespace clas12::geometry::preshower_cal;

    using namespace clas12::ccdb;
    using ::ccdb::Calibration;


    size_t nviews = 3;
    size_t nlayers = 5;
    double view_angle = 62.905;
    double wrapper_thick = 0.25;

    vector<size_t> nscint{68,62,62};
    vector<double> scint_width{15.01,6.0,22.0};



    /// constructor with arguments
    clog << "constructor...\n";

    ConnectionInfoMySQL conninfo;
    ConstantSetInfo csinfo;
    unique_ptr<Calibration> calib = get_calibration(conninfo,csinfo);

    PreshowerCal pcal(calib.get());

    for (size_t sec=0; sec<pcal.sectors().size(); sec++)
    {
        const preshower_cal::Sector& sector = pcal.sector(sec);

        BOOST_CHECK_EQUAL(sector.views().size(), nviews);

/*
        for (size_t iview=0; iview<sector.views().size(); iview++)
        {
            const forward_tof::Panel& panel = sector.panel(pan);

            clog << "sector " << sec << ", panel " << pan << endl;
            BOOST_CHECK_EQUAL(panel.paddles().size(), npaddles[pan]);
            BOOST_CHECK_CLOSE(panel.paddle_width(),paddle_width[pan], 0.0001 );
            BOOST_CHECK_CLOSE(panel.paddle_thickness(),paddle_thickness[pan], 0.0001 );
            BOOST_CHECK_CLOSE(panel.thtilt(),panel_thtilt[pan]*deg2rad, 0.0001 );
            BOOST_CHECK_CLOSE(panel.thmin(),panel_thmin[pan]*deg2rad, 0.0001 );
            BOOST_CHECK_CLOSE(panel.dist2edge(),panel_dist2edge[pan], 0.0001 );
            BOOST_CHECK_CLOSE(panel.paddle_gap(),panel_gap[pan], 0.0001 );
            //BOOST_CHECK_CLOSE(panel.paddle_pairgap(),panel_pairgap[pan], 0.0001 );
            BOOST_CHECK_CLOSE(panel.wrapper_thickness(),panel_wrapperthick[pan], 0.0001 );
            //clog<<panel.paddle_center_x(0)<<"  "<<panel.paddle_center_x(11)<<"  "<<
              //  panel.paddle_center_z(0)<<"  "<<panel.paddle_center_z(11)<<
                //"  "<<panel.radial_extent()<<endl;
        }
        */
    }

}

BOOST_AUTO_TEST_SUITE_END()

