#include "clas12/magfield/magfield_service.hpp"

#include <algorithm>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>

#include "PropertyList.hpp"

#include "clas12/magfield/request.hpp"

namespace clas12
{
namespace magfield
{

using std::clog;
using std::endl;

using std::string;
using std::stringstream;
using std::vector;

using std::runtime_error;

using boost::to_lower_copy;

const string MagFieldService::name =
    "CLAS12MagFieldService";
const string MagFieldService::desc =
R"(The CLAS12 Magnetic Field Service. The expected input must be a
PropertyList. Output will be an error message (string) or
the magnetic field as an byte buffer.

This service expects the field maps to be here:
    /group/clas12/clara_services/data/magfield

And the files to be called:
    clas12_torus_fieldmap_binary.dat
    solenoid-srr.dat

)" + Request::desc;
const string MagFieldService::author =
    "Johann Goetz";
const string MagFieldService::version =
    "0.0.1";

string MagFieldService::usage() const
{
    stringstream ss;
    ss << this->name << " version " << this->version << endl
       << "written by: " << this->author << endl
       << endl
       << this->desc << endl
       << endl;
    return ss.str();
}

CioSerial::UniquePtr MagFieldService::executeService(const PropertyList& plist)
{
    CioSerial::UniquePtr out;

    try
    {
        auto p = plist.findProperty("request");
        if (p != plist.end())
        {
            Request req(p->getValue());
            vector<unsigned char> buff = req.generate_buffer();

            out->setData(buff, MimeType::BYTE_ARRAY);
            out->setDataDescription("Byte Buffer");
            out->setStatus("Success");
        }
        else
        {
            out->setData( string("rejected: ") + \
                "input property-list did not have a request\n" + \
                this->usage(), MimeType::STRING );
            out->setDataDescription("Error message");
            out->setStatus("Error");
        }
    }
    catch (runtime_error& e)
    {
        out->setData(e.what(), MimeType::STRING);
        out->setDataDescription("Error message");
        out->setStatus("Error");
    }

    return out;
}

CioSerial::UniquePtr MagFieldService::executeService(const CioSerial::SharedPtr& in)
{
    CioSerial::UniquePtr out;
    switch (in->getMimeType())
    {
        case MimeType::PROPERTY_LIST:
        {
            PropertyList pl = in->getPropertyList();
            clog << "input property list: " << pl.getPackedString() << endl;
            out = this->executeService(pl);
            break;
        }
        default:
        {
            string expected_mime_type = mimeTypeToString(MimeType::PROPERTY_LIST);
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

} // namespace clas12::magfield
} // namespace clas12

namespace clara
{

using ::clas12::magfield::MagFieldService;

extern "C"
std::unique_ptr<CService> createService()
{
    return make_unique<MagFieldService>();
}

} // namespace clara
