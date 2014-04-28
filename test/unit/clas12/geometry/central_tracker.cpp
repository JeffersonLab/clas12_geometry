#define BOOST_TEST_DYN_LINK

#define BOOST_TEST_MODULE clas12_geometry_central_tracker

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

#include "clas12/geometry/central_tracker.hpp"
#include "clas12/geometry/output/bst_strip_endpoints.hpp"

BOOST_AUTO_TEST_SUITE(clas12_geometry_bst)

BOOST_AUTO_TEST_CASE(constructor)
{
    static const double deg2rad = 3.14159265358979 / 180.;
    using namespace std;
    using ::geometry::line;
    using ::geometry::plane;
    using namespace clas12::geometry;
    using namespace clas12::geometry::central_tracker;

    string ccdb_host = "clasdb.jlab.org";
    string ccdb_user = "clas12reader";
    string ccdb_db = "clas12";

    size_t nregions = 3; // regions that make up the BST
    vector<size_t> nsectors{10,14,18};   // sectors per region
    size_t nlayers   = 2; // layers  per sector
    size_t nstrips = 256; // number of strips in a region/sector/layer
    vector<double> region_phi{0,0,10*deg2rad,0};

    {
        /// constructor with arguments
        CentralTracker ct(ccdb_host,ccdb_user,ccdb_db);
        const BarrelSVT& bst = ct.bst();

        BOOST_CHECK_EQUAL(bst.regions().size(), nregions);

        //for (size_t reg=0; reg<bst.regions().size(); reg++)
        for (size_t reg = 0; reg<1; reg++)
        {
            const barrel_svt::Region& region = *bst.regions()[reg];
            BOOST_CHECK_EQUAL(region.sectors().size(), nsectors[reg]);
            BOOST_CHECK_CLOSE(region.phi(),region_phi[reg], 0.0001 );

            vector<plane<double>> sector_planes;

            //for (size_t sec=0; sec<region.sectors().size(); sec++)
            for (size_t sec = 0; sec < 1; sec++)
            {
                const barrel_svt::Sector& sector = *region.sectors().at(sec);
                BOOST_CHECK_EQUAL(sector.layers().size(), nlayers);

                sector_planes.push_back(sector.detector_plane());

                //for (size_t lyr=0; lyr<sector.layers().size(); lyr++)
                for (size_t lyr = 0; lyr<1; lyr++)
                {
                    const barrel_svt::Layer& layer = *sector.layers().at(lyr);
                    BOOST_CHECK_EQUAL(layer.strips().size(), nstrips);
                }
            }

            clog << "region " << reg << endl;
            clog << "nsectors " << region.sectors().size() << endl;

            for (auto this_plane =  sector_planes.begin();
                      this_plane != sector_planes.end();
                    ++this_plane )
            {
                auto adj_plane = this_plane;
                ++adj_plane;
                if (adj_plane == sector_planes.end())
                {
                    adj_plane = sector_planes.begin();
                }

                line<double,3> intrsct = this_plane->intersection(*adj_plane);

                //clog << "    " << this_plane->point() << ", " << this_plane->normal() << endl;

                //clog << "    " << this_plane->point() << ", " << this_plane->normal() << endl;
                //clog << "    " << adj_plane->point() << ", " << adj_plane->normal() << endl;

                double x = intrsct.point().x();
                double y = intrsct.point().y();
                x = fabs(x) < 1e-10 ? 0 : x;
                y = fabs(y) < 1e-10 ? 0 : y;
                clog << x << " " << y << endl;
            }
        }
    }
}


BOOST_AUTO_TEST_CASE(bst_strip_endpoints)
{
    using namespace std;
    using namespace geometry;
    using namespace clas12::geometry;
    using namespace clas12::geometry::central_tracker;

    using namespace clas12::geometry::output;
    using namespace pugi;

    static const double double_tolerance = 1.e-8;

    string ccdb_host = "clasdb.jlab.org";
    string ccdb_user = "clas12reader";
    string ccdb_db = "clas12";

    CentralTracker ct(ccdb_host, ccdb_user, ccdb_db);
    const BarrelSVT& bst = ct.bst();
    xml_document doc;
    stringstream doc_ss;

    bst_strip_endpoints_xml(doc,bst,"LAB","cm");
    doc.save(doc_ss);
    string endpoints_xml = doc_ss.str();
    cout<<endpoints_xml<<endl;
    BOOST_CHECK_EQUAL(endpoints_xml.size(), 1235243);
}

BOOST_AUTO_TEST_SUITE_END()

/*
BOOST_AUTO_TEST_SUITE(clas12_geometry_fst)

BOOST_AUTO_TEST_CASE(constructor)
{
    using namespace std;
    using namespace clas12::geometry;
    using namespace clas12::geometry::central_tracker;
    using namespace clas12::geometry::central_tracker::barrel_svt;

    string ccdb_host = "clasdb.jlab.org";
    string ccdb_user = "clasuser";
    string ccdb_db = "clas12";

    size_t nregions = 4;      // regions that make up the FST

}

BOOST_AUTO_TEST_SUITE_END()
*/
