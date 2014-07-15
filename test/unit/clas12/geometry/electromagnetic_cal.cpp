#define BOOST_TEST_DYN_LINK

#define BOOST_TEST_MODULE clas12_geometry_electromagnetic_cal

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

#include "clas12/geometry/electromagnetic_cal.hpp"
//#include "clas12/geometry/output/ec_volumes.hpp"

#include "clas12/ccdb/constants_table.hpp"

BOOST_AUTO_TEST_SUITE(clas12_geometry_ec)

BOOST_AUTO_TEST_CASE(constructor)
{
    static const double deg2rad = 3.14159265358979 / 180.;
    //static const double in2cm   = 1./2.54;

    using namespace std;
    using ::geometry::line;
    using ::geometry::plane;
    using namespace clas12::geometry;
    using namespace clas12::geometry::electromagnetic_cal;
    //using namespace clas12::geometry::output;

    using namespace clas12::ccdb;
    using ::ccdb::Calibration;
    using namespace pugi;
    xml_document doc;
    stringstream doc_ss;

    size_t nviews        = 3;
    size_t nlayers       = 13;
    double thtilt        = 25.;
    double alum_thick    = 3.81;
    double view_angle    = 62.88;
    double dist2tgt      = 712.723;
    double dist2cnt      = 95.088;
    double wrapper_thick = 0.025;
    double scint_thick   = 1.0;
    double lead_thick    = 0.2381;
    double a1            = 0.00856;
    double a2            = 186.46;
    double a3            = 0.445635;
    double d2            = 3.64;
    double d2prime       = 2.54;

    vector<size_t> nstrips{36,36,36};
    vector<double> a4{0.43708,0,0};
    vector<double> a5{10.366,9.4701,9.4926};
    vector<double> a6{0.02476,0.02256,0.02256};

    /// constructor with arguments
    clog << "constructor...\n";

    ConnectionInfoMySQL conninfo;
    ConstantSetInfo csinfo;
    unique_ptr<Calibration> calib = get_calibration(conninfo,csinfo);

    ElectromagneticCal ec(calib.get());




    for (size_t sec=0; sec<ec.sectors().size(); sec++)
    {
        const electromagnetic_cal::Sector& sector = ec.sector(sec);

        BOOST_CHECK_EQUAL(sector.nlayers(), nlayers);
        BOOST_CHECK_CLOSE(sector.thtilt(),thtilt*deg2rad, 0.0001 );
        BOOST_CHECK_CLOSE(sector.alum_thick(),alum_thick, 0.0001 );
        BOOST_CHECK_CLOSE(sector.dist2tgt(),dist2tgt, 0.0001 );
        BOOST_CHECK_CLOSE(sector.dist2cnt(),dist2cnt, 0.0001 );
        BOOST_CHECK_CLOSE(sector.a1(),a1, 0.0001 );
        BOOST_CHECK_CLOSE(sector.a2(),a2, 0.0001 );
        BOOST_CHECK_CLOSE(sector.a3(),a3, 0.0001 );
        BOOST_CHECK_CLOSE(sector.d2(),d2, 0.0001 );
        BOOST_CHECK_CLOSE(sector.d2prime(),d2prime, 0.0001 );

        for (size_t lyr=0; lyr<sector.layers().size(); lyr++)
        {
           const electromagnetic_cal::Layer& layer = sector.layer(lyr);


            clog << "sector " << sec << ", layer " << lyr << endl;
            BOOST_CHECK_EQUAL(layer.views().size(), nviews);
            BOOST_CHECK_CLOSE(layer.view_angle(),view_angle*deg2rad, 0.0001 );
            BOOST_CHECK_CLOSE(layer.wrapper_thick(),wrapper_thick, 0.0001 );
            BOOST_CHECK_CLOSE(layer.strip_thick(),scint_thick, 0.0001 );
            BOOST_CHECK_CLOSE(layer.lead_thick(),lead_thick, 0.0001 );

            for (size_t iview=0; iview<layer.nviews(); iview++)
            {
              const electromagnetic_cal::View& view = layer.view(iview);
              clog << "sector " << sec << ", layer " << lyr << ", view "<<iview<<endl;

              BOOST_CHECK_EQUAL(view.nstrips(),nstrips[iview]);
              BOOST_CHECK_CLOSE(view.shift(),a4[iview], 0.0001 );
              BOOST_CHECK_CLOSE(view.halfwidth(),a5[iview], 0.0001 );
              BOOST_CHECK_CLOSE(view.deltahw(),a6[iview], 0.0001 );
            }

        }



    }


    //ec_volumes_xml(doc,ec);
    //doc.save(doc_ss);
    //string scints_xml = doc_ss.str();
    //string volumes_xml = doc_ss.str();
    //cout<<scint_xml<<endl;
    //cout<<volumes_xml<<endl;
    //BOOST_CHECK_EQUAL(scint_xml.size(), 1235243);

    //ec_volumes_map(ec);
}









BOOST_AUTO_TEST_SUITE_END()

