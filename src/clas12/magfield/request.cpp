#include "request.hpp"

#include <cstdint>
#include <iostream>
#include <fstream>
#include <stdexcept>
#include <string>
#include <vector>

#include <boost/algorithm/string.hpp>

namespace clas12
{
namespace magfield
{

using std::boolalpha;
using std::noboolalpha;
using std::clog;
using std::endl;

using std::ifstream;
using std::ios;

using std::string;
using std::vector;

using std::runtime_error;

using boost::to_lower_copy;

const string Request::desc =
R"(Returns the magnetic field map as a byte array.

Input options:
    request (only one of the following per request):
              solenoid
              torus

Output type:
    Byte array of magnetic field
)";

Request::Request(const string& req)
{
    request = to_lower_copy(req);
    string clara_services_dir = "/group/clas12/clara_services";
    string data_dir = clara_services_dir + "/data";
    string magfield_dir = data_dir + "/magfield";
    magfield_files["torus"] = magfield_dir + "/clas12_torus_fieldmap_binary.dat";
    magfield_files["solenoid"] = magfield_dir + "/solenoid-srr.dat";
}

vector<byte> Request::generate_buffer()
{
    if (request == "torus" || request == "solenoid")
    {
        return this->generate_buffer(magfield_files[request]);
    }
    throw runtime_error("Error: no magnetic field: " + request + "\n" + Request::desc);
}

vector<byte> Request::generate_buffer(const string& filename)
{
    ifstream fin(filename, ios::binary);

    if (fin.is_open())
    {
        // get the data size in bytes
        fin.seekg(0, ios::end);
        size_t file_size = fin.tellg();
        fin.seekg(0, ios::beg);

        // read the data into buffer
        std::vector<byte> buff(file_size);
        fin.read(reinterpret_cast<char*>(&buff[0]), file_size);

        return buff;
    }
    throw runtime_error("could not open file: " + filename);
}

string Request::info()
{
    string msg = "Request: " + request;
    return msg;
}

} // namespace clas12::magfield
} // namespace clas12
