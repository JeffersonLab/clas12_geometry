#include <iostream>
#include <string>
#include <sstream>

#include "CCDB/Calibration.h"
#include "CCDB/MySQLCalibration.h"
#include "CCDB/Providers/MySQLDataProvider.h"

using namespace std;
using namespace ccdb;

int main()
{
    string user = "clas12reader";
    string host = "clasdb.jlab.org";
    string port = "3306";
    string db   = "clas12";

    /// forms the string: "mysql://clasuser@clasdb.jlab.org:3306 clas12"
    stringstream conn_ss;
    conn_ss << "mysql://" << user
        << "@" << host << ":" << port
        << "/" << db;
    string constsetid_str = "/geometry/dc/region";

    MySQLDataProvider calib;
    calib.Connect(conn_ss.str());

    Assignment* assignment = calib.GetAssignmentFull(100,constsetid_str);

    vector<vector<string>> tabledValues = assignment->GetData();
    vector<string> columns = assignment->GetTypeTable()->GetColumnNames();
    vector<string> column_types = assignment->GetTypeTable()->GetColumnTypeStrings();

    cout << endl;
    for (int i=0; i<tabledValues.size(); i++)
    {
        for (int j=0; j<tabledValues[i].size(); j++)
        {
            cout << " " << tabledValues[i][j];
        }
        cout << endl;
    }
}
