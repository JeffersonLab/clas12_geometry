#include <iostream>
using std::cout;
using std::endl;

#include "request.hpp"

#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>

#include "pugixml.hpp"

#include "clas12/geometry.hpp"
#include "clas12/geometry/coordsys.hpp"
#include "clas12/geometry/output.hpp"

namespace clas12
{
namespace geometry
{

using std::boolalpha;
using std::noboolalpha;
using std::clog;
using std::endl;

using std::stringstream;

using std::runtime_error;

using boost::split;
using boost::is_any_of;
using boost::lexical_cast;
using boost::to_lower_copy;

using pugi::xml_document;

const string Request::desc =
R"(Returns the expanded geometry parameters as an XML string.
The request can be a list (comma or space separated) of those
listed below.

Note that volumes are always in CLAS coordinates and in cm, and
therefore the units and coordsys arguments are ignored for
requests like dc/volumes

Input options:
    units:    (m|cm|mm)         [default: cm]
    coordsys: (clas|sector)     [default: clas]
    request:
              dc/wire_endpoints
              dc/volumes
              dc/core_params
              ftof/panels_parms
              ftof/volumes

Output type:
    XML string
)";

Request::Request(const map<string,string>& req)
{
    // defaults
    units = "cm";
    coords = "clas";

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

        static const string ccdb_host = "clasdb.jlab.org";
        static const string ccdb_user = "clas12reader";
        static const string ccdb_db = "clas12";

        xml_document doc;

        for (const auto& req : request)
        {
            string sys = req.first;

            if (sys == "dc")
            {
                DriftChamber dc(ccdb_host, ccdb_user, ccdb_db);

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
            else if (sys == "ftof")
            {
                ForwardTOF ftof(ccdb_host, ccdb_user, ccdb_db);

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

        static const string ccdb_host = "clasdb.jlab.org";
        static const string ccdb_user = "clas12reader";
        static const string ccdb_db = "clas12";

        volmap_t vmap;

        for (const auto& req : request)
        {
            string sys = req.first;

            if (sys == "dc")
            {
                DriftChamber dc(ccdb_host, ccdb_user, ccdb_db);

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
                ForwardTOF ftof(ccdb_host, ccdb_user, ccdb_db);
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
    string msg = "Request:\n";

    msg += "  coords: " + coords + "\n";
    msg += "  units:  " + units + "\n";

    for (auto i : request)
    {
        msg += "  system: " + i.first + "\n";
        for (auto j : i.second)
        {
            msg += "    " + j + "\n";
        }
    }

    return msg;
}

} // namespace clas12::geometry
} // namespace clas12
