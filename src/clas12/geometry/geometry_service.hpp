#ifndef __CLAS12_GEOMETRY_GEOMETRY_SERVICE_HPP__
#define __CLAS12_GEOMETRY_GEOMETRY_SERVICE_HPP__

#include <cstdint>
#include <list>
#include <map>
#include <string>
#include <vector>

#include "CService.hpp"

#include "clas12/geometry.hpp"

namespace clas12
{
namespace geometry
{

using std::list;
using std::map;
using std::string;
using std::vector;

using namespace clara;

class GeometryService : public CService
{
  private:
    static const string name;
    static const string desc;
    static const string author;
    static const string version;

    string usage() const;

    CioSerial::UniquePtr executeService(const PropertyList&);

    void configure(const CioSerial::SharedPtr&) {};
    CioSerial::UniquePtr executeService(
        const vector<CioSerial::SharedPtr>&) {};

  public:
    CioSerial::UniquePtr executeService(const CioSerial::SharedPtr&);

    string getName()        const { return GeometryService::name;    };
    string getDescription() const { return GeometryService::desc;    };
    string getAuthor()      const { return GeometryService::author;  };
    string getVersion()     const { return GeometryService::version; };
};

} // namespace clas12::geometry
} // namespace clas12

#endif // __CLAS12_GEOMETRY_GEOMETRY_SERVICE_HPP__
