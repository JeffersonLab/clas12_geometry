#include <iostream>
#include <string>

#include "geometry/line_segment.hpp"
#include "clas12/geometry/drift_chamber.hpp"

using namespace std;
using namespace clas12::geometry;

using geometry::line_segment;

int main()
{
    string host = "clasdb.jlab.org";
    string user = "clas12reader";
    string database = "clas12";
    DriftChamber dc(host,user,database);

    cout << "indexes run from 1 (not 0)\n";

    for (auto sec : vector<int>{0,2})
    {
        const drift_chamber::Sector& sector = dc.sector(sec);

        cout << "sector " << sec+1 << endl;

        for (auto reg : vector<int>{0})
        {
            const drift_chamber::Region& region = sector.region(reg);

            cout << "    region " << reg+1 << endl;

            for (auto slyr : vector<int>{0,1})
            {
                const drift_chamber::Superlayer& superlayer = region.superlayer(slyr);

                cout << "        superlayer " << slyr+1 << endl;

                for (auto lyr : vector<int>{0,5})
                {
                    const drift_chamber::Senselayer& layer = superlayer.senselayer(lyr);

                    cout << "            sense layer " << lyr+1 << endl;

                    for (auto wr : vector<int>{9,10,11})
                    {
                        line_segment<double,3> wire = layer.wire(wr);

                        cout << "                wire " << wr+1
                             << ":   "
                             << "(" << wire.begin_point().x()
                             << "," << wire.begin_point().y()
                             << "," << wire.begin_point().z()
                             << ")    "
                             << "(" << wire.end_point().x()
                             << "," << wire.end_point().y()
                             << "," << wire.end_point().z()
                             << ")\n";
                    }
                }
            }
        }
    }
}
