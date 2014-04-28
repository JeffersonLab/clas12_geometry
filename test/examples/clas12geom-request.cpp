#include <iostream>
#include <fstream>
#include <string>

#include "CioSerial.hpp"
#include "CService.hpp"
#include "PropertyList.hpp"

using namespace std;
using namespace clara;

class GeometryRequester : public CService {
  public:
    string plist_str;

    GeometryRequester() : CService("gemc", "ifarm1101.jlab.org", "CLAS12_PLATFORM")
    //GeometryRequester() : CService("gemc", "ucla.jlab.org", "GoetzPlatform")
    {
        //clog << "executing request\n";
        this->executeService("",0,NULL,NULL);
    };

    void executeService(
        string     requesting_service,
        int        type,
        CioSerial* in,
        CioSerial* out )
    {
        //clog << "creating cioSerial object...";
        CioSerial input;
        CioSerial output;

        //clog << "creating plist object...";
        PropertyList plist_in;
        plist_in.add("requesting_service", "sot");
        plist_in.add("system", "dc");

        //clog << "setting plist to CioSerial object...";
        string plist_in_str = plist_in.getPackedString();
        input.setData(plist_in_str, MimeType::STRING);

        //clog << "setting mimetype...";
        input.setMimeType("text/property-list");

        //clog << "setting description...";
        input.setDescription("none");

        {
            /// testing plist_in
            PropertyList plist_test;
            plist_test.initFromPackedString(plist_in_str);
            clog << "\ntesting plist: " << plist_test.getString("system") << endl;
        }

        //clog << "requesting geometry from Geometry Service...";
        this->requestService("CLAS12 Geometry Service", 0, &input, &output);

        PropertyList plist_out;
        if (output.getMimeType() == "text/property-list")
        {
            plist_out.initFromPackedString(output.getData());
        }

        plist_str = plist_out.getPackedString();


{
    PropertyList p = plist_out.childPropertyList("gemc.dc");
    clog << endl;
    clog << "variables for gemc.dc\n\n";
    clog << "region1.delx1: " << p.getFloat("region1.delx1") << endl;
    clog << "region2.delx1: " << p.getFloat("region2.delx1") << endl;
    clog << "region3.delx1: " << p.getFloat("region3.delx1") << endl;
    clog << endl;
    clog << "region1.dely: " << p.getFloat("region1.dely") << endl;
    clog << "region2.dely: " << p.getFloat("region2.dely") << endl;
    clog << "region3.dely: " << p.getFloat("region3.dely") << endl;
    clog << endl;
    clog << "region1.center_m.x: " << p.getFloat("region1.center_m.x") << endl;
    clog << "region1.center_m.y: " << p.getFloat("region1.center_m.y") << endl;
    clog << "region1.center_m.z: " << p.getFloat("region1.center_m.z") << endl;
    clog << endl;
    clog << "region2.center_m.x: " << p.getFloat("region2.center_m.x") << endl;
    clog << "region2.center_m.y: " << p.getFloat("region2.center_m.y") << endl;
    clog << "region2.center_m.z: " << p.getFloat("region2.center_m.z") << endl;
    clog << endl;
    clog << "region3.center_m.x: " << p.getFloat("region3.center_m.x") << endl;
    clog << "region3.center_m.y: " << p.getFloat("region3.center_m.y") << endl;
    clog << "region3.center_m.z: " << p.getFloat("region3.center_m.z") << endl;
    clog << endl;

    p = plist_out.childPropertyList("sot.dc");
    clog << "wireplanedist.region0.superlayer1: " << p.getFloat("wireplanedist.region0.superlayer1") << endl;
    clog << "wireplanedist.region1.superlayer1: " << p.getFloat("wireplanedist.region1.superlayer1") << endl;
    clog << "wireplanedist.region2.superlayer1: " << p.getFloat("wireplanedist.region2.superlayer1") << endl;
    clog << endl;
}
    }

    void configureService(vector<string>* a, vector<int32_t>* b, vector<string>* c) {};

};

int main(int argc, char** argv)
{

    int strlength = 0;

    for (int i=0; i<1; i++)
    {
        GeometryRequester geomreq;

        if (strlength != geomreq.plist_str.size())
        {
            clog << "Got first event or something is wrong!\n";
            strlength = geomreq.plist_str.size();
        }
        clog << "event count: " << i << "\n";

        //cout << geomreq.plist_str;
    }
    clog << "done.\n";

}
