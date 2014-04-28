#include <string>
#include <unistd.h>

#include "clas12/geometry/geometry_service.hpp"

using namespace clas12::geometry;

int main(int argc, char** argv)
{
    string platform_host = "ucla.jlab.org";
    string platform_name = "GoetzPlatform";
    //string platform_host = "129.57.28.27";
    //string platform_name = "CLAS12_PLATFORM";

    GeometryService theService(platform_host, platform_name);

    while (true)
    {
        sleep(5);
    }
}
