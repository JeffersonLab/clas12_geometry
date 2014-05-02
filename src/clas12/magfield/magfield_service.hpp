#ifndef CLAS12_MAGFIELD_MAGFIELD_SERVICE_HPP
#define CLAS12_MAGFIELD_MAGFIELD_SERVICE_HPP

#include <string>
#include <vector>

#include "CService.hpp"

namespace clas12
{
namespace magfield
{

using std::string;
using std::vector;

using namespace clara;

class MagFieldService : public CService
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

    string getName()        const { return MagFieldService::name;    };
    string getDescription() const { return MagFieldService::desc;    };
    string getAuthor()      const { return MagFieldService::author;  };
    string getVersion()     const { return MagFieldService::version; };
};

} // namespace clas12::magfield
} // namespace clas12

#endif // CLAS12_MAGFIELD_MAGFIELD_SERVICE_HPP
