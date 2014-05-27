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
    vector<double> scint_width_u;
    vector<double> scint_width_vw;

    scint_width_u.assign(52,4.5);    for (int i=0; i<16; i++) scint_width_u.push_back(9.05);
    scint_width_vw.assign(15,9.05);  for (int i=0; i<46; i++) scint_width_vw.push_back(4.5);

    /// constructor with arguments
    clog << "constructor...\n";

    ConnectionInfoMySQL conninfo;
    ConstantSetInfo csinfo;
    unique_ptr<Calibration> calib = get_calibration(conninfo,csinfo);

    PreshowerCal pcal(calib.get());

    for (size_t sec=0; sec<pcal.sectors().size(); sec++)
    {
        const preshower_cal::Sector& sector = pcal.sector(sec);

        BOOST_CHECK_EQUAL(sector.layers().size(), nlayers);


        for (size_t lyr=0; lyr<sector.layers().size(); lyr++)
        {
           const preshower_cal::Layer& layer = sector.layer(lyr);


            clog << "sector " << sec << ", layer " << lyr << endl;
            BOOST_CHECK_EQUAL(layer.views().size(), nviews);
            BOOST_CHECK_CLOSE(layer.view_angle(),view_angle*deg2rad, 0.0001 );
            BOOST_CHECK_CLOSE(layer.wrapper_thick(),wrapper_thick*0.1, 0.0001 );
        }

    }

}





/*
BOOST_AUTO_TEST_CASE(pcal_scint_width)
{
    using namespace std;
    using namespace geometry;
    using namespace clas12::geometry;
    using namespace clas12::geometry::preshower_cal;

    using namespace clas12::geometry::output;
    using namespace pugi;

    static const double double_tolerance = 1.e-8;

    string ccdb_host = "clasdb.jlab.org";
    string ccdb_user = "clas12reader";
    string ccdb_db = "clas12";

    PreshowerCal pcal(ccdb_host,ccdb_user,ccdb_db);
    xml_document doc;
    stringstream doc_ss;



    for (size_t sec=0; sec<pcal.sectors().size(); sec++)
    {
        const preshower_cal::Sector& sector = pcal.sector(sec);

        for (size_t lyr=0; lyr<sector.layers().size(); lyr++)
        {
            const preshower_cal::Layer& layer = sector.layer(lyr);

            for (size_t iview=0; iview<layer.nviews(); iview++)
            {
                layer._views.emplace_back(new View(&layer,iview));
                View& view = *layer._views[iview];

                BOOST_CHECK_CLOSE(view._strips().size(),nscint[iview], 0.0001 );

                 if (view.name() == "u")
                 {
                   BOOST_CHECK_CLOSE(view._strips_width(),scint_width_u, 0.0001 );
                 }

                 else if (view.name() == "v" || view.name() == "w")
                 {
                   BOOST_CHECK_CLOSE(view._strips_width(),scint_width_vw, 0.0001 );
                 }
            }
        }
    }



    //ftof_scint_parms_xml(doc,pcal,"clas","cm");
    //pcal_volumes_xml(doc,pcal);
    //doc.save(doc_ss);
    //string scints_xml = doc_ss.str();
    //string volumes_xml = doc_ss.str();
    //cout<<scint_xml<<endl;
    //cout<<volumes_xml<<endl;
    //BOOST_CHECK_EQUAL(scint_xml.size(), 1235243);

    //pcal_volumes_map(pcal);
}

*/







BOOST_AUTO_TEST_SUITE_END()

