#include "coordsys.hpp"

#include <stdexcept>

namespace clas12
{
namespace geometry
{

namespace COORDSYS
{
    using std::logic_error;

    coordsys_t str2coord(const string& coord)
    {
        if (coord == "clas")
        {
            return CLAS;
        }
        else if (coord == "sector")
        {
            return SECTOR;
        }
        else if (coord == "layer")
        {
            return LAYER;
        }
        else
        {
            throw logic_error("unknown coordinate system: "+coord);
        }
    }

    string coord2str(coordsys_t coord)
    {
        switch(coord)
        {
            case CLAS:   return "clas";
            case SECTOR: return "sector";
            case LAYER:  return "layer";
        }
    }
}

} // namespace clas12::geometry
} // namespace clas12
