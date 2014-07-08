#define BOOST_TEST_DYN_LINK

#define BOOST_TEST_MODULE clas12_geometry_forward_tof

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

#include "clas12/geometry/forward_tof.hpp"
#include "clas12/geometry/output/ftof_panels_parms.hpp"
#include "clas12/geometry/output/ftof_volumes.hpp"

#include "clas12/ccdb/constants_table.hpp"

BOOST_AUTO_TEST_SUITE(clas12_geometry_ftof)

BOOST_AUTO_TEST_CASE(constructor)
{
    static const double deg2rad = 3.14159265358979 / 180.;
    //static const double in2cm   = 1./2.54;

    using namespace std;
    using ::geometry::line;
    using ::geometry::plane;
    using namespace clas12::geometry;
    using namespace clas12::geometry::forward_tof;
    using namespace clas12::ccdb;
    using ::ccdb::Calibration;


    size_t npanels = 3;
    vector<size_t> npaddles{23,62,5};
    vector<double> paddle_width{15.01,6.0,22.0};
    vector<double> paddle_thickness{5.08,6.0,5.08};
    vector<double> panel_thtilt{25.0,25.0,58.11};
    vector<double> panel_thmin{5.453,3.667,34.698};
    vector<double> panel_dist2edge{726.689,717.236,659.71};
    vector<double> panel_gap{0.1384,0.04,0.302};
    vector<double> panel_pairgap{0.0,0.20,0.0};
    vector<double> panel_wrapperthick{0.0290,0.0122,0.0290};


    /// constructor with arguments
    clog << "constructor...\n";
    //ForwardTOF ftof(ccdb_host,ccdb_user,ccdb_db);
    ConnectionInfoMySQL conninfo;
    ConstantSetInfo csinfo;
    unique_ptr<Calibration> calib = get_calibration(conninfo,csinfo);

    ForwardTOF ftof(calib.get());

    for (size_t sec=0; sec<ftof.sectors().size(); sec++)
    {
        const forward_tof::Sector& sector = ftof.sector(sec);

        BOOST_CHECK_EQUAL(sector.panels().size(), npanels);

        for (size_t pan=0; pan<sector.panels().size(); pan++)
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
    }

}



BOOST_AUTO_TEST_CASE(ftof_paddle_centers)
{
    using namespace std;
    using namespace geometry;
    using namespace clas12::geometry;
    using namespace clas12::geometry::forward_tof;

    using namespace clas12::ccdb;
    using ::ccdb::Calibration;
    using namespace clas12::geometry::output;
    using namespace pugi;

    static const double double_tolerance = 1.e-8;


    //ForwardTOF ftof(ccdb_host,ccdb_user,ccdb_db);

    ConnectionInfoMySQL conninfo;
    ConstantSetInfo csinfo;
    unique_ptr<Calibration> calib = get_calibration(conninfo,csinfo);
    ForwardTOF ftof(calib.get());

    xml_document doc;
    stringstream doc_ss;



    for (size_t sec=0; sec<ftof.sectors().size(); sec++)
    {
        const forward_tof::Sector& sector = ftof.sector(sec);

        for (size_t pan=0; pan<sector.panels().size(); pan++)
        {
            const forward_tof::Panel& panel = sector.panel(pan);

            for (size_t pad=0; pad<panel.npaddles(); pad++)
            {
                //clog << sec << " " << pan << " " << pad << " " << panel.paddle_center(pad) << endl;
            }
        }
    }



    //ftof_paddle_parms_xml(doc,ftof,"clas","cm");
    //ftof_plane_parms_xml(doc,ftof,"clas","cm");
    ftof_volumes_xml(doc,ftof);
    doc.save(doc_ss);
    //string paddles_xml = doc_ss.str();
    //string panels_xml = doc_ss.str();
    string volumes_xml = doc_ss.str();
    //cout<<paddles_xml<<endl;
     //cout<<panels_xml<<endl;
    //cout<<volumes_xml<<endl;
    //BOOST_CHECK_EQUAL(paddles_xml.size(), 1235243);

    ftof_volumes_map(ftof);
}

BOOST_AUTO_TEST_SUITE_END()

