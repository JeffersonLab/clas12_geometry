#define BOOST_TEST_DYN_LINK

#define BOOST_TEST_MODULE clas12_geometry_drift_chamber

#include <boost/test/unit_test.hpp>

#include <iostream>
using std::clog;
using std::endl;

#include <array>
#include <cstddef>
#include <fstream>
#include <map>
#include <sstream>
#include <string>
#include <vector>

#include "pugixml.hpp"

#include "geometry.hpp"
#include "geometry/io.hpp"

#include "clas12/geometry/drift_chamber.hpp"
#include "clas12/geometry/output/dc_wire_endpoints.hpp"
#include "clas12/geometry/output/dc_volumes.hpp"
#include "clas12/ccdb/constants_table.hpp"

BOOST_AUTO_TEST_SUITE(clas12_geometry_drift_chamber)

BOOST_AUTO_TEST_CASE(constructor)
{
    using namespace std;
    using namespace clas12::geometry;
    using namespace clas12::geometry::drift_chamber;
    using namespace clas12::ccdb;
    using ::ccdb::Calibration;


    size_t nsectors = 6;      // sectors that make up the DC
    size_t nregions = 3;      // regions     per sector
    size_t nsuperlayers = 2;  // superlayers per region
    size_t nsenselayers = 6;  // senselayers per superlayer
    size_t nguardlayers = 2;  // guardlayers per superlayer
    size_t nsensewires = 112; // sensewires  per senselayer
    size_t nguardwires = 2;   // guardwires  per senselayer

    {

        /// constructor with arguments
        ConnectionInfoMySQL conninfo;
        ConstantSetInfo csinfo;
        unique_ptr<Calibration> calib = get_calibration(conninfo,csinfo);
        DriftChamber dcgeom(calib.get());

        BOOST_CHECK_EQUAL(dcgeom.sectors().size(), nsectors);

        for (size_t sec=0; sec<dcgeom.sectors().size(); sec++)
        {
            const Sector& sector = *dcgeom.sectors().at(sec);
            BOOST_CHECK_EQUAL(sector.regions().size(), nregions);

            for (size_t reg=0; reg<sector.regions().size(); reg++)
            {
                const Region& region = *sector.regions()[reg];
                BOOST_CHECK_EQUAL(region.superlayers().size(), nsuperlayers);

                for(size_t slyr; slyr<region.superlayers().size(); slyr++)
                {
                    const Superlayer& superlayer = region.superlayer(slyr);
                    BOOST_CHECK_EQUAL(superlayer.senselayers().size(), nsenselayers);
                    BOOST_CHECK_EQUAL(superlayer.guardlayers().size(), nguardlayers);

                    for (size_t lyr; lyr<superlayer.senselayers().size(); lyr++)
                    {
                        const Senselayer& senselayer = superlayer.senselayer(lyr);
                        BOOST_CHECK_EQUAL(senselayer.sensewires().size(), nsensewires);
                        BOOST_CHECK_EQUAL(senselayer.nguardwires(), nguardwires);
                    }

                    for (size_t lyr; lyr<superlayer.guardlayers().size(); lyr++)
                    {
                        const Guardlayer& guardlayer = superlayer.guardlayer(lyr);
                        BOOST_CHECK_EQUAL(guardlayer.nwires(), nsensewires+nguardwires);
                    }
                }
            }
        }
    }

    {
        /// default constructor
        DriftChamber dcgeom;
        {
            /// loading parameters in sub-scope
            //dcgeom.fetch_nominal_parameters(ccdb_host,ccdb_user,ccdb_db);
        }

        BOOST_CHECK_EQUAL(dcgeom.sectors().size(), nsectors);

        for (size_t sec=0; sec<dcgeom.sectors().size(); sec++)
        {
            const Sector& sector = *dcgeom.sectors().at(sec);
            BOOST_CHECK_EQUAL(sector.regions().size(), nregions);

            for (size_t reg=0; reg<sector.regions().size(); reg++)
            {
                const Region& region = *sector.regions()[reg];
                BOOST_CHECK_EQUAL(region.superlayers().size(), nsuperlayers);

                for(size_t slyr; slyr<region.superlayers().size(); slyr++)
                {
                    const Superlayer& superlayer = region.superlayer(slyr);
                    BOOST_CHECK_EQUAL(superlayer.senselayers().size(), nsenselayers);
                    BOOST_CHECK_EQUAL(superlayer.guardlayers().size(), nguardlayers);

                    for (size_t lyr; lyr<superlayer.senselayers().size(); lyr++)
                    {
                        const Senselayer& senselayer = superlayer.senselayer(lyr);
                        BOOST_CHECK_EQUAL(senselayer.sensewires().size(), nsensewires);
                        BOOST_CHECK_EQUAL(senselayer.nguardwires(), nguardwires);
                    }

                    for (size_t lyr; lyr<superlayer.guardlayers().size(); lyr++)
                    {
                        const Guardlayer& guardlayer = superlayer.guardlayer(lyr);
                        BOOST_CHECK_EQUAL(guardlayer.nwires(), nsensewires+nguardwires);
                    }
                }
            }
        }
    }
}

BOOST_AUTO_TEST_CASE(drift_chamber)
{
    using namespace std;
    using namespace clas12::geometry;
    using namespace clas12::geometry::drift_chamber;
    using namespace clas12::ccdb;
    using ::ccdb::Calibration;

    static const double double_tolerance = 1.e-8;

/// begin testing drift chamber methods ////////////////////////////////
    ConnectionInfoMySQL conninfo;
    ConstantSetInfo csinfo;
    unique_ptr<Calibration> calib = get_calibration(conninfo,csinfo);
    DriftChamber dcgeom(calib.get());

   //DriftChamber dcgeom(ccdb_host,ccdb_user,ccdb_db);

/// end testing drift chamber methods //////////////////////////////////
}

BOOST_AUTO_TEST_CASE(sector)
{
    using namespace std;
    using namespace clas12::geometry;
    using namespace clas12::geometry::drift_chamber;
    using namespace clas12::ccdb;
    using ::ccdb::Calibration;

    static const double double_tolerance = 1.e-8;

    ConnectionInfoMySQL conninfo;
    ConstantSetInfo csinfo;
    unique_ptr<Calibration> calib = get_calibration(conninfo,csinfo);
    DriftChamber dcgeom(calib.get());

    for (size_t sec=0; sec<dcgeom.sectors().size(); sec++)
    {
/// begin testing sector methods ///////////////////////////////////////
const Sector& sector = dcgeom.sector(sec);

/// end testing sector methods /////////////////////////////////////////
    }
}

BOOST_AUTO_TEST_CASE(region)
{
    using namespace std;
    using namespace clas12::geometry;
    using namespace clas12::geometry::drift_chamber;
    using namespace clas12::ccdb;
    using ::ccdb::Calibration;

    static const double double_tolerance = 1.e-8;

    ConnectionInfoMySQL conninfo;
    ConstantSetInfo csinfo;
    unique_ptr<Calibration> calib = get_calibration(conninfo,csinfo);
    DriftChamber dcgeom(calib.get());

    for (size_t sec=0; sec<dcgeom.sectors().size(); sec++)
    {
        const Sector& sector = dcgeom.sector(sec);

        for (size_t reg=0; reg<sector.regions().size(); reg++)
        {
/// begin testing region methods ///////////////////////////////////////
const Region& region = sector.region(reg);

/// thickness()
//BOOST_CHECK_CLOSE(region.thickness(), thickness[reg], double_tolerance);

/// end testing region methods /////////////////////////////////////////
        }
    }
}

BOOST_AUTO_TEST_CASE(superlayer)
{
    using namespace std;
    using namespace clas12::geometry;
    using namespace clas12::geometry::drift_chamber;
    using namespace clas12::ccdb;
    using ::ccdb::Calibration;

    static const double double_tolerance = 1.e-8;

    ConnectionInfoMySQL conninfo;
    ConstantSetInfo csinfo;
    unique_ptr<Calibration> calib = get_calibration(conninfo,csinfo);
    DriftChamber dcgeom(calib.get());


    int nwplanes = 22;
    int nlayers = 8;
    double cellthickness = 3.;

    /// wpdist[reg][slyr]
    vector<vector<double>> wpdist{
            {0.3862, 0.4042}, {0.6219, 0.6586}, {0.8600, 0.9000} };
    //vector<double> w0mid_dist2tgt{};
    //vector<double> w0mid_x{};
    //vector<double> w0mid_z{};
    //vector<double> wmid_spacing{};


    for (size_t sec=0; sec<dcgeom.sectors().size(); sec++)
    {
        const Sector& sector = dcgeom.sector(sec);

        for (size_t reg=0; reg<sector.regions().size(); reg++)
        {
            const Region& region = sector.region(reg);

            for(size_t slyr=0; slyr<region.superlayers().size(); slyr++)
            {
/// begin testing superlayer methods ///////////////////////////////////
const Superlayer& superlayer = region.superlayer(slyr);

/// nwireplanes()
BOOST_CHECK_EQUAL(superlayer.nwireplanes(), nwplanes);

/// thickness()
BOOST_CHECK_CLOSE(superlayer.thickness(), (nlayers-1)*cellthickness*wpdist[reg][slyr], double_tolerance);

/// dist2tgt()
double expected_dist2tgt = region.dist2tgt();
expected_dist2tgt += slyr * (
    region.midgap()
    + double(nlayers-1) * cellthickness * region.superlayer(0).wpdist() );
BOOST_CHECK_CLOSE(
    superlayer.dist2tgt(),
    expected_dist2tgt,
    double_tolerance );

/// first_wire_mid_dist2tgt()
//BOOST_CHECK_CLOSE(superlayer.first_wire_mid_dist2tgt(), w0mid_dist2tgt[reg][slyr], double_tolerance);

/// first_wire_mid_x()
//BOOST_CHECK_CLOSE(superlayer.first_wire_mid_x(), w0mid_x[reg][slyr], double_tolerance);

/// first_wire_mid_z()
//BOOST_CHECK_CLOSE(superlayer.first_wire_mid_z(), w0mid_z[reg][slyr], double_tolerance);

/// wire_mid_spacing()
//BOOST_CHECK_CLOSE(superlayer.wire_mid_spacing(), wmid_spacing[reg][slyr], double_tolerance);

/// end testing superlayer methods /////////////////////////////////////
            }
        }
    }
}

BOOST_AUTO_TEST_CASE(senselayer)
{
    using namespace std;
    using namespace clas12::geometry;
    using namespace clas12::geometry::drift_chamber;
    using namespace clas12::ccdb;
    using ::ccdb::Calibration;

    static const double double_tolerance = 1.e-8;

    size_t nguardlayers = 2;  // guardlayers per superlayer
    size_t nsensewires = 112; // sensewires  per senselayer
    size_t nwires = nguardlayers + nsensewires;

    static const double wmid_y = 0.;

    /// first wire x and z position for each (super?)layer
    /// delta-x and delta-z between each wire for each (super?)layer
    //vector<double> w0mid_x{};
    //vector<double> wmid_dx{};
    //vector<double> w0mid_z{};
    //vector<double> wmid_dz{};

    //vector<double> dist2tgt{};

    ConnectionInfoMySQL conninfo;
    ConstantSetInfo csinfo;
    unique_ptr<Calibration> calib = get_calibration(conninfo,csinfo);
    DriftChamber dcgeom(calib.get());


    for (size_t sec=0; sec<dcgeom.sectors().size(); sec++)
    {
        const Sector& sector = dcgeom.sector(sec);

        for (size_t reg=0; reg<sector.regions().size(); reg++)
        {
            const Region& region = sector.region(reg);

            for(size_t slyr=0; slyr<region.superlayers().size(); slyr++)
            {
                const Superlayer& superlayer = region.superlayer(slyr);

                for (size_t lyr=0; lyr<superlayer.senselayers().size(); lyr++)
                {
/// begin testing senselayer methods ///////////////////////////////////
const Senselayer& senselayer = superlayer.senselayer(lyr);

size_t abs_lyr = lyr
               + slyr * superlayer.senselayers().size()
               + reg  * region.superlayers().size();

/// wire_mid_y()
BOOST_CHECK_CLOSE(senselayer.wire_mid_y(), wmid_y, double_tolerance);

//for (size_t wire=0; wire<nwires; wire++)
//{
    /// wire_mid_x(wire)
    //BOOST_CHECK_CLOSE(senselayer.wire_mid_x(), wmid_x, double_tolerance);
    /// wire_mid_z(wire)
    //BOOST_CHECK_CLOSE(senselayer.wire_mid_z(), wmid_z, double_tolerance);
//}

/// dist2tgt()
//BOOST_CHECK_CLOSE(senselayer.dist2tgt(), dist2tgt[abs_lyr], double_tolerance);

/// end testing senselayer methods /////////////////////////////////////
                }
            }
        }
    }
}

BOOST_AUTO_TEST_CASE(wire_endpoints)
{
    using namespace std;
    using namespace geometry;
    using namespace clas12::geometry;
    using namespace clas12::geometry::drift_chamber;
    using namespace clas12::ccdb;
    using ::ccdb::Calibration;
    static const double double_tolerance = 1.e-8;


    // index order: [sec][reg][slyr][lyr][wire]
    //multi_array<euclid_vector<double,3>,6,3,2,6,3> wire_begin_points = {
    //};
    //multi_array<euclid_vector<double,3>,6,3,2,6,3> wire_end_points = {
        //{},{},{},{},{},{}
    //};

    ConnectionInfoMySQL conninfo;
    ConstantSetInfo csinfo;
    unique_ptr<Calibration> calib = get_calibration(conninfo,csinfo);
    DriftChamber dcgeom(calib.get());


    // vols[name][var] = val
    map<string, map<string,string> > vols;

    clog << "{";

    for (size_t sec=0; sec<1 /*dcgeom.sectors().size()*/; sec++)
    {
        const Sector& sector = dcgeom.sector(sec);

        if (sec>0) { clog << ","; }
        clog << "{";

        for (size_t reg=0; reg<sector.regions().size(); reg++)
        {
            const Region& region = sector.region(reg);

            if (reg>0) { clog << ","; }
            clog << "{";

            for(size_t slyr=0; slyr<region.superlayers().size(); slyr++)
            {
                const Superlayer& superlayer = region.superlayer(slyr);

                if (slyr>0) { clog << ","; }
                clog << "{";

                //for (size_t lyr=0; lyr<superlayer.senselayers().size(); lyr++)
                for (size_t lyr : {0,1,2,5})
                {
/// begin testing senselayer methods ///////////////////////////////////
const Senselayer& senselayer = superlayer.senselayer(lyr);

if (lyr>0) { clog << ","; }
clog << "{";

for (int w : {0,47,113})
{
    line_segment<double,3> s = senselayer.wire(w);
    euclid_vector<> a = s.begin_point();

    if (w>0) { clog << ","; }
    clog << "{" << a.x() << "," << a.y() << "," << a.z() << "}";

}

clog << "}";

/// end testing senselayer methods /////////////////////////////////////
                }
                clog << "}";
            }
            clog << "}";
        }
        clog << "}";
    }
    clog << "};\n";
}


BOOST_AUTO_TEST_CASE(dc_wire_endpoints)
{
    using namespace std;
    using namespace geometry;
    using namespace clas12::geometry;
    using namespace clas12::geometry::drift_chamber;
    using namespace clas12::ccdb;
    using ::ccdb::Calibration;

    using namespace clas12::geometry::output;
    using namespace pugi;

    static const double double_tolerance = 1.e-8;

    ConnectionInfoMySQL conninfo;
    ConstantSetInfo csinfo;
    unique_ptr<Calibration> calib = get_calibration(conninfo,csinfo);
    DriftChamber dcgeom(calib.get());

    xml_document doc;
    stringstream doc_ss;

    dc_wire_endpoints_xml(doc,dcgeom,"sector","cm");
    doc.save(doc_ss);
    string endpoints_xml = doc_ss.str();

    BOOST_CHECK_EQUAL(endpoints_xml.size(), 1643450);
}

BOOST_AUTO_TEST_CASE(dc_volumes)
{
    using namespace std;
    using namespace geometry;
    using namespace clas12::geometry;
    using namespace clas12::geometry::drift_chamber;
    using namespace clas12::ccdb;
    using ::ccdb::Calibration;

    using namespace clas12::geometry::output;
    using namespace pugi;

    static const double double_tolerance = 1.e-8;

    ConnectionInfoMySQL conninfo;
    ConstantSetInfo csinfo;
    unique_ptr<Calibration> calib = get_calibration(conninfo,csinfo);
    DriftChamber dcgeom(calib.get());


    xml_document doc;
    stringstream doc_ss;

    dc_volumes_xml(doc,dcgeom);
    doc.save(doc_ss);

    string dc_volumes = doc_ss.str();

    clog << dc_volumes_xml << endl;

    BOOST_CHECK_EQUAL(dc_volumes.size(), 1235243);


}

BOOST_AUTO_TEST_SUITE_END()
