#ifndef CLAS12_GEOMETRY_REQUEST_HPP
#define CLAS12_GEOMETRY_REQUEST_HPP

#include <map>
#include <memory>
#include <string>
#include <vector>

#include "clas12/ccdb/constants_table.hpp"

namespace clas12
{
namespace geometry
{

using std::map;
using std::string;
using std::vector;

using ccdb::Calibration;
using clas12::ccdb::ConstantSetInfo;

typedef map<string, map<string,string> > volmap_t;

class Request
{
  public:
    static const string desc;

    Request(const map<string,string>& req);

    string generate_xml();
    volmap_t generate_volmap();

    string info();

  private:
    map<string,vector<string>> request;

    string coords;
    string units;

    unique_ptr<Calibration> calib;
    ConstantSetInfo csinfo;

    map<string,string> to_lower(const map<string,string>& req);
};

} // namespace clas12::geometry
} // namespace clas12

#endif // CLAS12_GEOMETRY_REQUEST_HPP
