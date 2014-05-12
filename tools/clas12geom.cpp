#include <algorithm>
#include <iostream>
#include <map>
#include <string>

#include "boost/program_options.hpp"

#include "clas12/geometry/request.hpp"

namespace po = boost::program_options;

using namespace std;

int main(int argc, char** argv)
{
    map<string,string> args;

    args["request"] = "";

    args["units"] = "cm";
    args["coordsys"] = "sector";

    args["run"] = "0";
    args["variation"] = "default";
    args["timestamp"] = "";

    args["sqlite"] = "";
    args["mysql-host"] = "clasdb.jlab.org";
    args["mysql-user"] = "clas12reader";
    args["mysql-password"] = "";
    args["mysql-database"] = "clas12";
    args["mysql-port"] = "3306";

    po::options_description options("Options");
    options.add_options()
        ("help,h",
            "produce help message")
        ("units,u",
            po::value<string>(&args["units"])->default_value(args["units"]),
            "units of length for all returned values")
        ("coordsys,c",
            po::value<string>(&args["coordsys"])->default_value(args["coordsys"]),
            "coordinate system")
        ("request,q",
            po::value<string>(&args["request"]),
            "system/values request string, comma separated. Example: dc/wire_endpoints,ftof/panels_parms")

        ("run,r",
            po::value<string>(&args["run"])->default_value(args["run"]),
            "run number")
        ("variation,v",
            po::value<string>(&args["variation"])->default_value(args["variation"]),
            "variation")
        ("timestamp,t",
            po::value<string>(&args["timestamp"]),
            "timestamp in the form YY-MM-DD hh:mm:ss")

        ("sqlite,f",
            po::value<string>(&args["sqlite"]),
            "SQLite file (may be relative path). Mutually exclusive with the MySQL options. If used, all MySQL options will be ignored.")
        ("mysql-host",
            po::value<string>(&args["mysql-host"]),
            "Host name of the MySQL server.")
        ("mysql-user",
            po::value<string>(&args["mysql-user"]),
            "User name to use when connectiong to the MySQL server.")
        ("mysql-password",
            po::value<string>(&args["mysql-password"]),
            "Password to use when connectiong to the MySQL server.")
        ("mysql-database",
            po::value<string>(&args["mysql-database"]),
            "Database name to use when connectiong to the MySQL server.")
        ("mysql-port",
            po::value<string>(&args["mysql-port"]),
            "Port number to use when connectiong to the MySQL server.")
    ;

    po::variables_map vm;
    po::store(po::command_line_parser(argc, argv).options(options).run(), vm);
    po::notify(vm);

    if (vm.count("help"))
    {
        clog << options << endl << endl;;
        clog << clas12::geometry::Request::desc;
        exit(0);
    }

    // sqlite take precedence over mysql
    if (args["sqlite"] != "")
    {
        for (auto i : vector<string>{"host", "user", "password", "database", "port"})
        {
            args.erase("mysql-"+i);
        }
    }
    else
    {
        args.erase("sqlite");
    }

    // remove all blank options
    vector<string> remove;
    for (auto i : args)
    {
        if (i.second == "")
        {
            remove.push_back(i.first);
        }
    }
    for (auto k : remove)
    {
        args.erase(k);
    }

    clas12::geometry::Request req(args);
    string xmlbuffer = req.generate_xml();

    clog << req.info() << endl;
    cout << xmlbuffer << endl;

    static const string err_str = "Error";
    if (equal(err_str.begin(), err_str.end(), xmlbuffer.begin()))
    {
        clog << options << endl << endl;;
        clog << clas12::geometry::Request::desc;
        exit(1);
    }
}
