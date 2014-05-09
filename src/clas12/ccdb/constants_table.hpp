#ifndef CLAS12_CCDB_CONSTANTS_TABLE_HPP
#define CLAS12_CCDB_CONSTANTS_TABLE_HPP

#include <iostream>
using std::clog;
using std::endl;

#include <algorithm>
#include <ctime>
#include <string>
#include <sstream>
#include <stdexcept>

#include <boost/filesystem.hpp>

#include "CCDB/CalibrationGenerator.h"
#include "CCDB/Calibration.h"

namespace clas12
{
namespace ccdb
{

namespace fs = boost::filesystem;

using std::string;
using std::stringstream;
using std::vector;
using std::time_t;
using std::time;

using ::ccdb::Calibration;
using ::ccdb::CalibrationGenerator;
using ::ccdb::Assignment;

typedef unsigned short int PortNumber;
typedef int RunNumber;
typedef int AssignmentID;
typedef vector<vector<string>> TableData;
typedef vector<string> ColumnNames;
typedef vector<string> ColumnTypes;

/** \brief combines the user, host and such into the MySQL connection
 * string used by MySQLCalibration::Connect().
 *
 * forms the string:
 *     "mysql://clas12reader@clasdb.jlab.org:3306/clas12"
 * by default.
 *
 * \return the MySQL connection string
 **/
class ConnectionInfoMySQL
{
  public:
    string user;
    string password;
    string host;
    PortNumber port;
    string database;

    ConnectionInfoMySQL()
    : user("clas12reader")
    , password("")
    , host("clasdb.jlab.org")
    , port(3306)
    , database("clas12")
    {}

    string connection_string() const
    {
        stringstream ss;
        ss << "mysql://";
        ss << user;
        if (password != "")
        {
            ss << ":" << password;
        }
        ss << "@" << host;
        ss << ":" << port;
        ss << "/" << database;
        return ss.str();
    }
};

/** \brief creates an SQLite connection string to be used by
 * SQLiteCalibration::Connect().
 *
 * forms the string:
 *     "sqlite:///clas12_ccdb.sqlite"
 * by default.
 *
 * \return the SQLite connection string
 **/
class ConnectionInfoSQLite
{
  public:
    string filepath;

    ConnectionInfoSQLite(string fp = "clas12_ccdb.sqlite")
    : filepath(fp)
    {}

    string connection_string() const
    {
        string ret;
        if (fs::exists(filepath))
        {
            if (fs::is_regular_file(filepath))
            {
                ret = "sqlite:///" + filepath;
            }
            else
            {
                throw range_error("ERROR: file '"+filepath+"' is not a regular file.");
            }
        }
        else
        {
            throw range_error("ERROR: file '"+filepath+"' could not be found.");
        }
        return ret;
    }
};


struct ConstantSetInfo
{
    string table_path;
    RunNumber run;
    string variation;
    time_t timestamp;

    ConstantSetInfo(string tp="", int r=INT_MAX, string v="default", time_t ts=0)
    : table_path(tp)
    , run(r)
    , variation(v)
    , timestamp(ts)
    {}

    string constant_set_string() const
    {
        stringstream ss;

        ss << table_path;

        if (run != INT_MAX)
        {
            ss << ":" << run;
        }
        else if (variation != "default" || timestamp != 0)
        {
            ss << ":";
        }

        if (variation != "default")
        {
            ss << ":" << timestamp;
        }
        else if (timestamp != 0)
        {
            ss << ":";
        }

        if (timestamp != 0)
        {
            ss << ":" << timestamp;
        }

        return ss.str();
    }
};

template <class ConnectionInfoType>
unique_ptr<Calibration> get_calibration(const ConnectionInfoType& conn)
{
    string connstr = conn.connection_string();
    CalibrationGenerator calibgen;
    return unique_ptr<Calibration>(calibgen.MakeCalibration(
        connstr, INT_MAX, "default", time(nullptr) ) );
}


template<class ConnectionInfoType>
AssignmentID get_assignment_id(
    Calibration* const calib,
    const ConstantSetInfo& constset)
{
    string constsetstr = constset.constant_set_string();

    bool disconnect = false;
    if (!calib->IsConnected())
    {
        disconnect = true;
        calib->Connect(calib->GetConnectionString());
    }

    unique_ptr<Assignment> assignment(calib->GetAssignment(constsetstr, true));
    AssignmentID id = assignment->GetId();

    if (disconnect)
    {
        calib->Disconnect();
    }

    return id;
}

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
    TableData values;

    /// the names of the columns
    ColumnNames columns;

    /// the types of the columns in string form
    ColumnTypes column_types;

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
    ConstantsTable(
        Calibration* const calib,
        const ConstantSetInfo& constset)
    {
        bool disconnect = false;
        if (!calib->IsConnected())
        {
            disconnect = true;
            calib->Connect(calib->GetConnectionString());
        }

        string constsetstr = constset.constant_set_string();
        unique_ptr<Assignment> assignment(calib->GetAssignment(constsetstr, true));
        values = assignment->GetData();
        columns = assignment->GetTypeTable()->GetColumnNames();
        column_types = assignment->GetTypeTable()->GetColumnTypeStrings();

        if (disconnect)
        {
            calib->Disconnect();
        }
    }

    ConstantsTable(const TableData& v, const ColumnNames& c, const ColumnTypes& ct)
    : values(v)
    , columns(c)
    , column_types(ct)
    {}

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
