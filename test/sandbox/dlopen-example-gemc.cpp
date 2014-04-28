#include <iostream>
#include <string>
#include <map>
#include <dlfcn.h>

using namespace std;

typedef map<string, map<string,string> > volmap_t;

int main()
{
    clog << "Opening library...\n";
    void* handle = dlopen("libclas12_geometry_gemc.so.0", RTLD_NOW);

    if (!handle)
    {
        cerr << "Cannot open library: " << dlerror() << '\n';
        return 1;
    }

    clog << "Loading symbol...\n";
    typedef volmap_t (*get_volume_maps_t)(const map<string,string>&);

    // reset errors
    dlerror();

    // extract function symbol from library
    get_volume_maps_t get_volume_maps = (get_volume_maps_t) dlsym(handle, "get_volume_maps");

    const char *dlsym_error = dlerror();
    if (dlsym_error)
    {
        cerr << "Cannot load symbol 'get_volume_maps': "
             << dlsym_error << '\n';
        dlclose(handle);
        return 1;
    }

    // use it to do the calculation
    clog << "Calling function...\n";

    volmap_t dc_volume_map;
    map<string,string> request;
    request["request"] = "dc/volumes";
    dc_volume_map = get_volume_maps(request);

    // close the library
    clog << "Closing library...\n";
    dlclose(handle);

    clog << "size of map: " << dc_volume_map.size() << endl;

    // use output of function
    volmap_t::const_iterator vol;
    map<string,string>::const_iterator par;

    for (vol = dc_volume_map.begin(); vol != dc_volume_map.end(); ++vol)
    {
        clog << vol->first << endl;

        for (par = vol->second.begin(); par != vol->second.end(); ++par)
        {
            clog << "    " << par->first << " = " << par->second << endl;
        }
    }
}
