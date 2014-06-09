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
    using namespace pugi;
    xml_document doc;
    stringstream doc_ss;

    size_t nviews = 3;
    size_t nlayers = 5;
    size_t nfoam  = 2;
    size_t nsteel = 4;
    double thtilt = 25;
    double foam_thick = 5.08;
    double steel_thick = 0.2;
    double view_angle = 62.905;
    double wrapper_thick = 0.025;
    double scint_thick = 1.0;
    double scint_width = 4.5;
    double lead_thick = 0.215;
    vector<size_t> nstrips{84,77,77};
    vector<double> max_length{394.2,432.7,432.7};

    /// constructor with arguments
    clog << "constructor...\n";

    ConnectionInfoMySQL conninfo;
    ConstantSetInfo csinfo;
    unique_ptr<Calibration> calib = get_calibration(conninfo,csinfo);

    PreshowerCal pcal(calib.get());

    for (size_t sec=0; sec<pcal.sectors().size(); sec++)
    {
        const preshower_cal::Sector& sector = pcal.sector(sec);

        BOOST_CHECK_EQUAL(sector.nlayers(), nlayers);
        BOOST_CHECK_EQUAL(sector.nfoam(), nfoam);
        BOOST_CHECK_EQUAL(sector.nsteel(), nsteel);
        BOOST_CHECK_CLOSE(sector.thtilt(),thtilt*deg2rad, 0.0001 );
        BOOST_CHECK_CLOSE(sector.foam_thick(),foam_thick, 0.0001 );
        BOOST_CHECK_CLOSE(sector.steel_thick(),steel_thick, 0.0001 );

        for (size_t lyr=0; lyr<sector.layers().size(); lyr++)
        {
           const preshower_cal::Layer& layer = sector.layer(lyr);


            clog << "sector " << sec << ", layer " << lyr << endl;
            BOOST_CHECK_EQUAL(layer.views().size(), nviews);
            BOOST_CHECK_CLOSE(layer.view_angle(),view_angle*deg2rad, 0.0001 );
            BOOST_CHECK_CLOSE(layer.wrapper_thick(),wrapper_thick, 0.0001 );
            BOOST_CHECK_CLOSE(layer.strip_thick(),scint_thick, 0.0001 );
            BOOST_CHECK_CLOSE(layer.strip_width(),scint_width, 0.0001 );
            BOOST_CHECK_CLOSE(layer.lead_thick(),lead_thick, 0.0001 );

            for (size_t iview=0; iview<layer.nviews(); iview++)
            {
              const preshower_cal::View& view = layer.view(iview);
              clog << "sector " << sec << ", layer " << lyr << ", view "<<iview<<endl;

              BOOST_CHECK_EQUAL(view.nstrips(),nstrips[iview]);
              BOOST_CHECK_CLOSE(view.max_length(),max_length[iview], 0.0001 );
            }

        }




    }


    //pcal_volumes_xml(doc,pcal);
    //doc.save(doc_ss);
    //string scints_xml = doc_ss.str();
    //string volumes_xml = doc_ss.str();
    //cout<<scint_xml<<endl;
    //cout<<volumes_xml<<endl;
    //BOOST_CHECK_EQUAL(scint_xml.size(), 1235243);

    //pcal_volumes_map(pcal);
}









BOOST_AUTO_TEST_SUITE_END()

