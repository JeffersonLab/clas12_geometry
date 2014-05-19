#include "clas12/geometry/geometry_service.hpp"

#include <algorithm>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>

#include "PropertyList.hpp"

#include "pugixml.hpp"

#include "clas12/geometry/request.hpp"

#include "clas12/log.hpp"

namespace clas12
{
namespace geometry
{

using std::boolalpha;
using std::noboolalpha;
using std::endl;

using std::string;
using std::stringstream;
using std::vector;
using std::equal;

using boost::split;
using boost::is_any_of;
using boost::lexical_cast;
using boost::to_lower_copy;

using pugi::xml_document;


const string GeometryService::name =
    "CLAS12GeometryService";
const string GeometryService::desc =
R"(The CLAS12 Geometry Service. The expected input must be a
PropertyList. Output will be an error message (string) or
the geometry parameters as an XML string.

)" + Request::desc;
const string GeometryService::author =
    "Johann Goetz, Yelena Prok";
const string GeometryService::version =
    "0.6.0";

string GeometryService::usage() const
{
    stringstream ss;
    ss << this->name << " version " << this->version << endl
       << "written by: " << this->author << endl
       << endl
       << this->desc << endl
       << endl;
    return ss.str();
}

CioSerial::UniquePtr GeometryService::executeService(const PropertyList& plist)
{
    // convert request from PropertyList to map<string,string>
    map<string,string> req;
    for (const auto& key : {"units", "coordsys", "request"})
    {
        auto p = plist.findProperty(key);
        if (p != plist.end())
        {
            req[key] = p->getValue();
        }
    }

    // initialize request
    Request request(req);

    LOG(info) << request.info();

    string out_str = request.generate_xml();

    LOG(info) << out_str << endl;

    // prepare output
    CioSerial::UniquePtr out;
    out = make_unique<CioSerial>();
    out->setData(out_str, MimeType::STRING);

    // the output must not begin with "Error"
    static const string err_str = "Error";
    if (equal(err_str.begin(), err_str.end(), out_str.begin()))
    {
        out->setDataDescription("Error message");
        out->setStatus("Error");
    }
    else
    {
        out->setDataDescription("XML string");
        out->setStatus("Success");
    }

    return out;
}

CioSerial::UniquePtr GeometryService::executeService(const CioSerial::SharedPtr& in)
{
    CioSerial::UniquePtr out;
    switch (in->getMimeType())
    {
        case MimeType::PROPERTY_LIST:
        {
            PropertyList pl = in->getPropertyList();
            LOG(info) << "input property list: " << pl.getPackedString() << endl;
            out = this->executeService(pl);
            break;
        }
        default:
        {
            string expected_mime_type = mimeTypeToString(MimeType::PROPERTY_LIST);
            LOG(error) << "    Bad input mime-type: "
                       << mimeTypeToString(in->getMimeType()) << endl;
            out = make_unique<CioSerial>();
            out->setData( string("rejected: ") + \
                "input was not of mime-type: " + \
                expected_mime_type + "\n" + \
                this->usage(), MimeType::STRING );
            out->setDataDescription("Error message");
            out->setStatus("Error");
            break;
        }
    }
    return out;
}

} // namespace clas12::geometry
} // namespace clas12

namespace clara
{

using ::clas12::geometry::GeometryService;

extern "C"
std::unique_ptr<CService> createService()
{
    return make_unique<GeometryService>();
}

} // namespace clara
