#ifndef CLAS12_MAGFIELD_REQUEST_HPP
#define CLAS12_MAGFIELD_REQUEST_HPP

#include <map>
#include <string>
#include <vector>

namespace clas12
{
namespace magfield
{

using std::map;
using std::string;
using std::vector;

typedef uint8_t byte;

class Request
{
  public:
    static const string desc;
    Request(const string& req);
    vector<byte> generate_buffer();
    string info();

  private:
    map<string,string> magfield_files;
    string request;
    vector<byte> generate_buffer(const string& filename);
};

} // namespace clas12::magfield
} // namespace clas12

#endif // CLAS12_MAGFIELD_REQUEST_HPP
