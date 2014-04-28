#include <iostream>
#include <fstream>
#include <string>

#include "CService.h"
#include "GooPropertyList.h"

namespace clas12
{

using namespace std;

class GeometryRequester : public CService {
  public:
    string plist_str;

    GeometryRequester(string requesting_service)
    : CService("GeometryRequester", "129.57.28.27", "CLAS12_PLATFORM")
    //: CService("gemc", "ucla.jlab.org", "GoetzPlatform")
    {
        clog << "creating cioSerial object...";
        CioSerial input;
        CioSerial output;

        clog << "creating plist object...";
        GooPropertyList plist_in;
        plist_in.add("requesting_service", requesting_service.c_str());
        plist_in.add("system", "dc");

        clog << "setting plist to CioSerial object...";
        string plist_in_str = plist_in.getPackedString();
        input.setData(plist_in_str, MimeType::STRING);

        clog << "setting mimetype...";
        input.setMimeType("text/property-list");

        clog << "setting description...";
        input.setDescription("none");

        {
            /// testing plist_in
            GooPropertyList plist_test;
            plist_test.initFromPackedString(plist_in_str);
            clog << "\ntesting plist: " << plist_test.getString("system") << endl;
        }

        clog << "requesting geometry from Geometry Service...";
        this->requestService("CLAS12 Geometry Service", 0, &input, &output);
        clog << "done.";

        GooPropertyList plist_out;
        if (output.getMimeType() == "text/property-list")
        {
            plist_out.initFromPackedString(output.getData());
        }

        plist_str = plist_out.getPackedString();
    };

    void executeService(
        string     requesting_service,
        int        type,
        CioSerial* in,
        CioSerial* out )
    {};
    void configureService(vector<string>* a, vector<int32_t>* b, vector<string>* c) {};

};

} /* namespace clas12 */

extern "C" {
    std::string get_dc_geometry(std::string requesting_service)
    {
        clas12::GeometryRequester geomreq(requesting_service);
        return geomreq.plist_str;
    }
}
