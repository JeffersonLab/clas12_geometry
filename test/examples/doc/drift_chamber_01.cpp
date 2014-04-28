#include <iostream>

#include "clas12/geometry/drift_chamber.hpp"

using std::clog;
using std::endl;

using clas12::geometry::DriftChamber;
using clas12::geometry::drift_chamber::Region;

int main()
{
    // create the DC geometry object and fetch
    // the nominal geometry from the CCDB database
    DriftChamber dc("clasdb.jlab.org", "clasuser", "clas12");

    // get a reference to the 3rd sector, 1st region
    // note that indexes are (always!) counted from zero
    // exceptions to this are considered bugs!
    /// [const ref to sub object]
    const Region& sec3reg1 = dc.sector(2).region(0);
    /// [const ref to sub object]

    // print the distance from the target to the 1st
    // superlayer in this region
    clog << "3rd sector, 1st region, 1st superlayer\n"
            "distance to target: " << sec3reg1.superlayer(0).dist2tgt()
            << endl;
}
