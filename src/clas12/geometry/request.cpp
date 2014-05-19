#include "request.hpp"

#include <iostream>
#include <ctime>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/regex.hpp>

#include "pugixml.hpp"

#include "clas12/geometry.hpp"
#include "clas12/geometry/coordsys.hpp"
#include "clas12/geometry/output.hpp"

#include "clas12/ccdb/parse_timestamp.hpp"

namespace clas12
{
namespace geometry
{

using std::boolalpha;
using std::noboolalpha;

using std::stringstream;
using std::ctime;

using std::runtime_error;
using std::invalid_argument;

using boost::regex;
using boost::split;
using boost::is_any_of;
using boost::lexical_cast;
using boost::to_lower_copy;

using pugi::xml_document;

using ccdb::ConnectionInfoMySQL;
using ccdb::ConnectionInfoSQLite;
using ccdb::ConstantSetInfo;

using clas12::ccdb::parse_timestamp;

const string Request::desc =
R"(Returns the expanded geometry parameters as an XML string.
The request can be a list (comma or space separated) of those
listed below.

Note that volumes are always in CLAS coordinates and in cm, and
therefore the units and coordsys arguments are ignored for
requests like dc/volumes

The run number, variation and timestamp default to the standard
CCDB defaults: i.e. the last run number, "default" variation,
and current time.

The sqlite and mysql options are mutually exclusive and not
all the mysql options need to be used - only those you wish
to change. The sqlite option is not set by default (mysql is
used) and the mysql-password is blank as the default user,
clas12reader, does not have an associated password.

Input options:
    units:    (m|cm|mm)         [default: cm]
    coordsys: (clas|sector)     [default: clas]
    request:
              dc/wire_endpoints
              dc/volumes
              dc/core_params
              ftof/panels_parms
              ftof/volumes

    run:      <int>             [default: <blank>]
    variation <string>          [default: default]
    timestamp <string>          [default: <blank>]

    sqlite:
              /path/to/file     [default: <not set>]

    mysql-host: hostname        [default: clasdb.jlab.org]
    mysql-user: username        [default: clas12reader]
    mysql-password: password    [default: <blank>]
    mysql-database: database    [default: clas12]
    mysql-port: portnumber      [default: 3306]

Output type:
    XML string
)";

Request::Request(const map<string,string>& req)
{
    // defaults
    units = "cm";
    coords = "clas";

    bool mysql_info = false;
    bool sqlite_info = false;

    ConnectionInfoMySQL conninfo_mysql;
    ConnectionInfoSQLite conninfo_sqlite;
    ConstantSetInfo csinfo;

    map<string,string> reqs = this->to_lower(req);

    for (const auto& r : reqs)
    {
        if (r.first == "units")
        {
            units = r.second;
        }
        else if (r.first == "coordsys")
        {
            coords = r.second;
        }
        else if (r.first == "request")
        {
            vector<string> items;
            split(items, r.second, is_any_of(" ,;:|"));

            for (const auto& i : items)
            {
                vector<string> item;
                split(item, i, is_any_of("/."));

                request[item[0]].push_back(item[1]);
            }
        }
        else if (r.first == "run")
        {
            try
            {
                csinfo.run = lexical_cast<int>(r.second);
            }
            catch (...)
            {
                throw invalid_argument("could not convert run number to integer.");
            }
        }
        else if (r.first == "variation")
        {
            csinfo.variation = r.second;
        }
        else if (r.first == "timestamp")
        {
            try
            {
                csinfo.timestamp = parse_timestamp(r.second);
            }
            catch (...)
            {
                throw invalid_argument("could not interpret timestamp.");
            }
        }
        else if (r.first == "sqlite")
        {
            sqlite_info = true;
            conninfo_sqlite.filepath = r.second;
        }
        else if (r.first == "mysql-user")
        {
            mysql_info = true;
            conninfo_mysql.user = r.second;
        }
        else if (r.first == "mysql-password")
        {
            mysql_info = true;
            conninfo_mysql.password = r.second;
        }
        else if (r.first == "mysql-host")
        {
            mysql_info = true;
            conninfo_mysql.host = r.second;
        }
        else if (r.first == "mysql-port")
        {
            mysql_info = true;
            try
            {
                conninfo_mysql.port = lexical_cast<int>(r.second);
            }
            catch (...)
            {
                throw invalid_argument("could not convert mysql port number to integer.");
            }
        }
    }

    if (sqlite_info && mysql_info)
    {
        throw invalid_argument("mysql and sqlite options are mutually exclusive.");
    }
    else if (sqlite_info)
    {
        this->calib = get_calibration(conninfo_sqlite, csinfo);
    }
    else // MySQL
    {
        this->calib = get_calibration(conninfo_mysql, csinfo);
    }
}

map<string,string> Request::to_lower(const map<string,string>& request)
{
    map<string,string> translated_request;
    for (auto& i : request)
    {
        translated_request[to_lower_copy(i.first)] = to_lower_copy(i.second);
    }
    return translated_request;
}


string Request::generate_xml()
{
    using namespace output;

    try
    {
        if (request.size() < 1)
        {
            throw runtime_error("request is empty.");
        }

        xml_document doc;

        for (const auto& req : request)
        {
            string sys = req.first;

            if (sys == "dc")
            {
                DriftChamber dc(calib.get());

                for (const auto& item : req.second)
                {
                    if (item == "wire_endpoints")
                    {
                        dc_wire_endpoints_xml(doc, dc, coords, units);
                    }
                    else if (item == "core_params")
                    {
                        dc_core_params_xml(doc, dc, units);
                    }
                    else if (item == "volumes")
                    {
                        dc_volumes_xml(doc, dc);
                    }
                    else
                    {
                        string err = "Error: Bad request for DC geometry:"
                            " dc/" + item +
                            ", " + coords + " coordinates"
                            ", " + units + "\n";
                        return err;
                    }
                }
            }
            else if (sys == "pcal")
            {
                PreshowerCal pcal(calib.get());

                for (const auto& item : req.second)
                {
                    if (item == "strip_endpoints")
                    {
                        //strip_wire_endpoints_xml(doc, pcal, coords, units);
                    }
                    else if (item == "core_params")
                    {
                        //pcal_core_params_xml(doc, pcal, units);
                    }
                    else if (item == "volumes")
                    {
                        //pcal_volumes_xml(doc, pcal);
                    }
                    else
                    {
                        string err = "Error: Bad request for PCAL geometry:"
                            " pcal/" + item +
                            ", " + coords + " coordinates"
                            ", " + units + "\n";
                        return err;
                    }
                }
            }
            else if (sys == "ftof")
            {
                ForwardTOF ftof(calib.get());

                for (const auto& item : req.second)
                {
                    if (item == "panels_parms")
                    {
                        ftof_panels_parms_xml(doc, ftof, coords, units);
                    }
                    else if (item == "volumes")
                    {
                        ftof_volumes_xml(doc, ftof);
                    }
                    else
                    {
                        string err = "Error: Bad request for FTOF geometry:"
                            " ftof/" + item +
                            ", " + coords + " coordinates"
                            ", " + units + "\n";
                        return err;
                    }
                }
            }
            else
            {
                string err = "Error: Bad request for geometry:"
                    " unknown system: " + sys + "\n";
                return err;
            }
        }

        stringstream doc_ss;
        doc.save(doc_ss); // serialize XML doc to string
        return doc_ss.str();
    }
    catch (runtime_error& e)
    {
        string err = "Error: Bad request for geometry.\n";
        err += string("Exception caught in clas12::geometry::Request::generate_xml():\n")
            + e.what();
        return err;
    }
    catch (...)
    {
        string err = R"(Error: unhandled exception caught in clas12::geometry::Request::generate_xml()
        This needs to be addressed by the geometry library developers.)";
        return err;
    }
}


volmap_t Request::generate_volmap()
{
    using namespace output;

    try
    {
        if (request.size() < 1)
        {
            throw runtime_error("request is empty.");
        }

        volmap_t vmap;

        for (const auto& req : request)
        {
            string sys = req.first;

            if (sys == "dc")
            {
                DriftChamber dc(calib.get());

                for (const auto& item : req.second)
                {
                    if (item == "volumes")
                    {
                        volmap_t tmp = dc_volumes_map(dc);
                        vmap.insert(tmp.begin(), tmp.end());
                    }
                    else
                    {
                        string err = "Error: Bad request for DC geometry:"
                            " dc/" + item + "\n";
                        throw runtime_error(err);
                    }
                }
            }
            else if (sys == "ftof")
            {
                cout << "fetching FTOF geometry...\n";
                ForwardTOF ftof(calib.get());
                cout << "done.\n";

                for (const auto& item : req.second)
                {

                    if (item == "volumes")
                    {
                        volmap_t tmp = ftof_volumes_map(ftof);
                        vmap.insert(tmp.begin(), tmp.end());
                    }
                    else
                    {
                        string err = "Error: Bad request for FTOF geometry:"
                            " ftof/" + item  + "\n";
                        throw runtime_error(err);
                    }
                }
            }
            else
            {
                string err = "Error: Bad request for geometry:"
                    " unknown system: " + sys + "\n";
                throw runtime_error(err);
            }
        }

        return vmap;
    }
    catch (runtime_error& e)
    {
        string err = "Error: Bad request for geometry.\n";
        err += string("Exception caught in clas12::geometry::Request::generate_volmap():\n")
            + e.what();
        throw runtime_error(err);
    }
    catch (...)
    {
        string err = R"(Error: unhandled exception caught in clas12::geometry::Request::generate_volmap()
        This needs to be addressed by the geometry library developers.)";
        throw runtime_error(err);
    }
}


string Request::info()
{
    stringstream ss;
    ss << "Request:\n";

    ss << "  coords: " << coords << "\n";
    ss << "  units:  " << units << "\n";

    for (auto i : request)
    {
        ss << "  system: " << i.first << "\n";
        for (auto j : i.second)
        {
            ss << "    " << j << "\n";
        }
    }

    ss << "  run: " << csinfo.run << "\n";
    ss << "  variation: " << csinfo.variation << "\n";
    ss << "  timestamp: " << ctime(&csinfo.timestamp);

    return ss.str();
}


} // namespace clas12::geometry
} // namespace clas12
