#include <iostream>
#include <fstream>
#include <string>

#include "CService.hpp"

namespace clas12 {

using namespace std;

class GeometryRequester : public CService {
  public:
    GeometryRequester() : CService("gemc", "129.57.28.27", "CLAS12_PLATFORM")
    //GeometryRequester() : CService("gemc", "ucla.jlab.org", "GoetzPlatform")
    {};

    void executeService()
    {
        clog << "requesting geometry from Geometry Service...";
        this->requestService("CLAS12 Geometry Service", 0, in, out);
    }

    void configureService(vector<string>* a, vector<int32_t>* b, vector<string>* c) {};

};

} /* namespace clas12 */


extern "C" clas12::GeometryRequester* create() {
    return new clas12::GeometryRequester;
}

extern "C" void destroy(clas12::GeometryRequester* p) {
    delete p;
}
