#include <map>
#include <string>

#include "clas12/geometry/output/dc_volumes.hpp"
#include "clas12/geometry/request.hpp"

using std::map;
using std::string;
using clas12::geometry::output::volmap_t;
using clas12::geometry::Request;

extern "C"
{

volmap_t get_volume_maps(const map<string,string>& request)
{
    static const string ccdb_host = "clasdb.jlab.org";
    static const string ccdb_user = "clas12reader";
    static const string ccdb_db = "clas12";

    Request req(request);
    return req.generate_volmap();
}

} // extern "C"
