#include <iostream>
#include <string>

#include <dlfcn.h>

#include "CService.hpp"
#include "MimeType.hpp"
#include "PropertyList.hpp"

using namespace std;
using namespace clara;

int main(int argc, char** argv)
{
    // load the geometry library
    string geomreqlib = "libclas12_geometry_requester.so";
    if (argc > 1)
    {
        geomreqlib = string(argv[1])+"/"+geomreqlib;
    }

    void* geomreq = dlopen(geomreqlib.c_str(), RTLD_LAZY);
    if (!geomreq) {
        cerr << "Cannot load library: " << dlerror() << "\n\n";
        cerr << "Usage:\n";
        cerr << argv[0] << " [libdir]\n\n";
        cerr << "where libdir is the directory where " << geomreqlib << " resides.\n";
        return 1;
    }

    // reset errors
    dlerror();

    // load the symbols
    typedef CService* create_t();
    typedef void destroy_t(CService*);

    create_t* create_geomreq = (create_t*) dlsym(geomreq, "create");
    const char* dlsym_error = dlerror();
    if (dlsym_error) {
        cerr << "Cannot load symbol create: " << dlsym_error << '\n';
        return 1;
    }

    destroy_t* destroy_geomreq = (destroy_t*) dlsym(geomreq, "destroy");
    dlsym_error = dlerror();
    if (dlsym_error) {
        cerr << "Cannot load symbol destroy: " << dlsym_error << '\n';
        return 1;
    }

    // create an instance of the class
    CService* geomreq_serv = create_geomreq();

    {
        clog << "creating plist object...";
        PropertyList plist_in;
        plist_in.addTailProperty("requesting_service", "gemc");
        plist_in.addTailProperty("system", "dc");

        // input data for CService
        clog << "creating input...";
        CioSerial input;
        input.setData(plist_in, MimeType::STRING);
        input.setDataDescription("none");

        clog << "executing service...";
        CioSerial::UniquePtr output = geomreq_serv->executeService(std::make_shared<CioSerial>(input));

        clog << "processing plist...";
        PropertyList plist_out;
        if (output->getMimeType() == MimeType::PROPERTY_LIST)
        {
            plist_out = PropertyList(output->getData());
        }

        cout << plist_out.getPackedString() << endl;
    }

    // destroy the class
    destroy_geomreq(geomreq_serv);

    // unload the geometry library
    dlclose(geomreq);


}
