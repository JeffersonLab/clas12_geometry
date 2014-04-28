#include <iostream>
#include <string>

#include <dlfcn.h>

using namespace std;

int main(int argc, char** argv)
{
    // load the triangle library
    string geomreqlib = "libclas12_geometry_requester_fn.so";
    if (argc > 1)
    {
        geomreqlib = string(argv[1])+"/"+geomreqlib;
    }

    void* geomreq = dlopen(geomreqlib.c_str(), RTLD_LAZY);
    if (!geomreq)
    {
        cerr << "Cannot load library: " << dlerror() << "\n\n";
        cerr << "Usage:\n";
        cerr << argv[0] << " [libdir]\n\n";
        cerr << "where libdir is the directory where " << geomreqlib << " resides.\n";
        return 1;
    }

    // load the symbol
    clog << "Loading symbol hello...\n";
    typedef string (*get_dc_geometry_t)(string);

    // reset errors
    dlerror();
    get_dc_geometry_t get_dc_geometry =
        (get_dc_geometry_t) dlsym(geomreq, "get_dc_geometry");

    const char *dlsym_error = dlerror();

    if (dlsym_error)
    {
        cerr << "Cannot load symbol 'get_dc_geometry': " << dlsym_error << '\n';
        dlclose(geomreq);
        return 1;
    }

    // use it to do the calculation
    cout << get_dc_geometry("gemc");

    // close the library
    clog << "Closing library...\n";
    dlclose(geomreq);
}
