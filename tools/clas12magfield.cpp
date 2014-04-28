#include <cstdlib>
#include <fstream>
#include <iostream>
#include <map>
#include <string>

#include "boost/program_options.hpp"

#include "clas12/magfield/request.hpp"

namespace po = boost::program_options;

using namespace std;

typedef uint8_t byte;

int main(int argc, char** argv)
{
    string request = "";
    string outfile = "out.dat";

    po::options_description options("Options");
    options.add_options()
        ("help,h",
            "produce help message.")
        ("request,r",
            po::value<string>(&request),
            "field map request string. Example: torus,solenoid")
        ("output,o",
            po::value<string>(&outfile)->default_value(outfile),
            "output file to put the field map.")
    ;

    po::variables_map vm;
    po::store(po::command_line_parser(argc, argv).options(options).run(), vm);
    po::notify(vm);

    if (vm.count("help") || request == "")
    {
        clog << options << endl << endl;;
        clog << clas12::magfield::Request::desc;
        exit(0);
    }

    clas12::magfield::Request req(request);
    clog << req.info() << endl;

    vector<byte> buff = req.generate_buffer();

    ofstream fout(outfile, ios::binary);
    fout.write(reinterpret_cast<char*>(&buff.front()), buff.size());
    fout.close();
}
