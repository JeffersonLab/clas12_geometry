#include <iostream>

#include "clas12/ccdb/constants_table.hpp"

using std::cout;
using std::endl;

int main()
{
    cout << "creating constants table object...\n";
    clas12::ccdb::ConstantsTable table;

    cout << "loading geometry constants for DC/region...\n";
    table.load_constants("/geometry/dc/region");


    /// looking at the table generically...
    cout << "col row colname coltype elem(col,row): default=double, int, float, string\n";
    cout << endl;
    for (int i=0; i<table.ncols(); i++)
    {
        for (int j=0; j<table.nrows(); j++)
        {

            cout << i << ", " << j
                << " " << table.colname(i)
                << " " << table.coltype(table.colname(i))
                << " " << table.elem(table.colname(i),j)
                << " " << table.elem<int>(table.colname(i),j)
                << " " << table.elem<float>(table.colname(i),j)
                << " " << table.elem<string>(table.colname(i),j)
                << endl;
        }
        cout << endl;
    }

    /// now we look at the table, knowing its from the
    /// /geometry/dc/region table in the database...

    cout << "xdist column type: " << table.coltype("xdist") << endl;

    /// for example, to get the xdist for region 2, we could
    /// get the row index from the method row() and use that
    /// in elem() to get the element "xdist" at that row.
    cout << "xdist, region 2: "
        << table.elem( "xdist", table.row("region",2) )
        << endl;

    /// Notice that getting the row index works for integers but
    /// not for floats!!! Use only integers or strings.
    /// This usage is OK, but not recommended.
    cout << "which region has dist2tgt==348.09: "
        << table.elem("region", table.row("dist2tgt", "348.09"))
        << endl;
}
