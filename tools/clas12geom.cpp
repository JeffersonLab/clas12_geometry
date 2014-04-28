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
    args["units"] = "cm";
    args["coordsys"] = "sector";
    args["request"] = "";

    po::options_description options("Options");
    options.add_options()
        ("help,h",
            "produce help message.")
        ("units,u",
            po::value<string>(&args["units"])->default_value(args["units"]),
            "units of length for all returned values.")
        ("coordsys,c",
            po::value<string>(&args["coordsys"])->default_value(args["coordsys"]),
            "coordinate system.")
        ("request,r",
            po::value<string>(&args["request"]),
            "system/values request string, comma separated. Example: dc/wire_endpoints,ftof/panels_parms")
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
