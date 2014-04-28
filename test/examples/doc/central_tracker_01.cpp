#include <iostream>

#include "clas12/geometry/central_tracker.hpp"

using namespace std;
using clas12::geometry::CentralTracker;
using namespace clas12::geometry::central_tracker;

int main()
{
    // create the SVT geometry object and fetch
    // the nominal geometry from the CCDB database
    CentralTracker ct("clasdb.jlab.org", "clas12reader", "clas12");

    // get a reference to the Barrel SVT
    /// [const ref to sub object]
    const BarrelSVT& bst = ct.bst();
    /// [const ref to sub object]

    const barrel_svt::Layer& lyr = bst.region(0).sector(5).layer(0);
    clog << "BST, 1st region, 6th sector, 1st layer:\n"
            "number of strips: " << lyr.strips().size() << endl;
}
