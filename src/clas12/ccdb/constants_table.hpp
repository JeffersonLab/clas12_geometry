#ifndef CLAS12_CCDB_CONSTANTS_TABLE_HPP
#define CLAS12_CCDB_CONSTANTS_TABLE_HPP

#include <iostream>
using std::clog;
using std::endl;

#include <algorithm>
#include <string>
#include <sstream>
#include <stdexcept>

#include "CCDB/Providers/DataProvider.h"

namespace clas12
{
namespace ccdb
{

using namespace ::ccdb;

using std::string;
using std::stringstream;
using std::vector;

/** \brief ConstantsTable is a conatiner class for any constants
 *  set. It will connect to the database when load_constants()
 *  is called. Columns can be accessed (and converted to specific
 *  types) by the col(colname) method. Individual numbers can be
 *  obtained via the elem<typename>(colname, row_index) method.
 **/
class ConstantsTable
{
  private:
    /// the table as filled by MySQLCalibration::GetCalib()
    vector<vector<string> > values;

    /// the names of the columns
    vector<string> columns;

    /// the types of the columns in string form
    vector<string> column_types;

    /// data provider object connecting to the database
    DataProvider* dataprovider;

    /** \brief find the index of the column associated with the name
     *  colname.
     *
     * \return column index of the column identified by colname
     **/
    unsigned int find_column(const string& colname)
    {
        return distance(
            columns.begin(),
            find(columns.begin(), columns.end(), colname) );
    }

    /** \brief generic function to convert a string to any type (T)
     *
     **/
    template <typename T>
    T lexical_cast(const string& str)
    {
        T ret;
        if (!(stringstream(str) >> ret))
        {
            stringstream ss;
            ss << "Could not convert: '" << str << "' to a numeric type.";
            throw std::logic_error(ss.str());
        }
        return ret;
    }

  public:

    ConstantsTable(ccdb::DataProvider* dp)
    {
        dataprovider = dp;
    }

    /** \brief combines the user, host and such into the MySQL connection
     * string used by MySQLCalibration::Connect().
     *
     * forms the string: "mysql://clas12reader@clasdb.jlab.org:3306/clas12"
     * by default.
     *
     * \return the MySQL connection string
     **/
    string mysql_connection_string(
        const string& user = "clas12reader",
        const string& passwd = "",
        const string& host = "clasdb.jlab.org",
        const string& port = "3306",
        const string& db   = "clas12" )
    {
        // forms the string: "mysql://clas12reader@clasdb.jlab.org:3306/clas12"
        stringstream conn_ss;
        conn_ss << "mysql://" << user;
        if (passwd != "")
        {
            conn_ss << ":" << passwd;
        }
        conn_ss << "@" << host << ":" << port
                << "/" << db;
        return conn_ss.str();
    }

    /** \brief creates an SQLite connection string
     *
     * \return the SQLite connection string
     **/
    string sqlite_connection_string(const string& filepath = "clas12_ccdb.sqlite")
    {
        // forms the string: "sqlite:///path/to/clas12_ccdb.sqlite"
        stringstream conn_ss;
        conn_ss << "sqlite://" << filepath;
        return conn_ss.str();
    }

    /** \brief builds the string to identify the constant set in CCDB
     *
     **/
    string constant_set_string(
        const string& path,
        int run = INT_MAX,
        const string& variation = "default",
        const string& timestamp = "" )
    {
        stringstream ss;

        ss << path;

        if (run != INT_MAX)
        {
            ss << ":" << run;
        }
        else if (variation != "default" || timestamp != "")
        {
            ss << ":";
        }

        if (variation != "default")
        {
            ss << ":" << timestamp;
        }
        else if (timestamp != "")
        {
            ss << ":";
        }

        if (timestamp != "")
        {
            ss << ":" << timestamp;
        }

        return ss.str();
    }

    /** \brief clears all data in this set.
     *
     **/
    void clear()
    {
        values.clear();
        columns.clear();
        column_types.clear();
    }

    /** \brief connects to the database and obtains the
     * data, the column names, and their types.
     **/
    void load_constants(
        const string& path,
        int run = INT_MAX,
        const string& variation = "default",
        const string& timestamp = "" )
    {
        this->clear();

        bool disconnect = false;
        if (!dataprovider->IsConnected())
        {
            dataprovider->Connect();
            disconnect = true;
        }

        string constsetid_str = this->constant_set_string(path,run,variation,timestamp);
        Assignment* assignment = calib.GetAssignmentFull(100,constsetid_str);
        if (!assignment) { return; }

        values = assignment->GetData();
        columns = assignment->GetTypeTable()->GetColumnNames();
        column_types = assignment->GetTypeTable()->GetColumnTypeStrings();

        if (disconnect)
        {
            dataprovider->Disconnect();
        }
    }

    /** \brief connects to the database and obtains the
     * data, the column names, and their types.
     **/
    void load_constants(int assignment_id)
    {
        this->clear();

        bool disconnect = false;
        if (!dataprovider->IsConnected())
        {
            dataprovider->Connect();
            disconnect = true;
        }

        Assignment* assignment = calib.GetAssignmentFull(assignment_id);
        if (!assignment) { return; }

        values = assignment->GetData();
        columns = assignment->GetTypeTable()->GetColumnNames();
        column_types = assignment->GetTypeTable()->GetColumnTypeStrings();

        if (disconnect)
        {
            dataprovider->Disconnect();
        }
    }

    /** \return number of rows in this data set.
     *
     **/
    unsigned int nrows()
    {
        return values.size();
    }

    /** \return number of columns in this data set.
     *
     **/
    unsigned int ncols()
    {
        if (this->nrows() > 0)
        {
            return values.at(0).size();
        }
        else
        {
            return 0;
        }
    }

    /** \return the column name of the ith column
     *
     **/
    string colname(const unsigned int& i)
    {
        return columns.at(i);
    }

    /** \return the column type of the ith column
     *
     **/
    string coltype(const unsigned int& i)
    {
        return column_types.at(i);
    }

    /** \return the column type of the column identified by colname
     *
     **/
    string coltype(const string& colname)
    {
        return coltype(find_column(colname));
    }

    /** \brief specialization of coltype() for const char*
     *
     * \return the column type of the column identified by colname
     **/
    string coltype(const char* colname)
    {
        return coltype(string(colname));
    }

    /** \return vector<T=double> of a column identified by the
     *  colname.
     **/
    template <typename T=double>
    vector<T> col(const string& colname)
    {
        vector<T> ret;

        unsigned int col_index = find_column(colname);

        for (int row_index=0; row_index<nrows(); row_index++)
        {
            ret.push_back(lexical_cast<T>(values.at(row_index).at(col_index)));
        }

        return ret;
    }

    /** \brief finds the element in the table associated with column
     * identified by colname and row specified (default row: 0)
     *
     *  typical usage:
     * for example, to get the xdist for region 2, we could get the
     * row index from the method row() and use that as in a call
     * elem() to get the element "xdist" at that row.
     *
     * ConstantsTable table;
     * table.load_constants("/geometry/dc/region");
     * table.elem( "xdist", table.row("region",2) );
     *
     * Equivalently, if we already know that the table is ordered
     * properly and the index associated with region 2 is 2-1 = 1,
     * then we could do this to get the same number as above:
     *
     * table.elem("xdist", 1);
     *
     *  \return element of the table cast to type T (default: double)
     **/
    template <typename T=double>
    T elem(const string& colname, const unsigned int& row=0)
    {
        return lexical_cast<T>(
            values.at(row).at(find_column(colname)) );
    }

    /** \brief find the first row of a specified column that has a
     * value that equals val.
     *
     * NOTE: getting the row index works for integers but
     * not for floats!!! Use only integers or strings. The
     * "string for floats" usage is OK, but not recommended.
     *
     * example:
     *   table.row("dist2tgt", "348.09")
     *
     * \return row index associated with the value val
     **/
    template <typename T>
    unsigned int row(const string& colname, const T& val)
    {
        for (unsigned int i=0; i<nrows(); i++)
        {
            if (elem<T>(colname,i) == val)
            {
                return i;
            }
        }
        stringstream ss;
        ss << "No such value: " << val << " in column " << colname;
        throw std::logic_error(ss.str());
    }

    /** \brief specialization for char* (converting to string)
     *
     **/
    unsigned int row(const string& colname, const char* val)
    {
        return row<string>(colname, string(val));
    }

};

} // namespace clas12::ccdb
} // namespace clas12

#endif // CLAS12_CCDB_CONSTANTS_TABLE_HPP
