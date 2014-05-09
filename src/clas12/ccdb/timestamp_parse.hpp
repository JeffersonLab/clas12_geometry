#ifndef CLAS12_CCDB_TIMESTAMP_PARSE_HPP
#define CLAS12_CCDB_TIMESTAMP_PARSE_HPP

#include <algorithm>
#include <cctype>
#include <ctime>
#include <string>

#include <boost/xpressive/xpressive.hpp>
#include <boost/lexical_cast.hpp>


namespace clas12
{
namespace ccdb
{

using namespace std;
using namespace boost::xpressive;

using std::string;
using std::tm;
using std::time_t;
using std::mktime;
using std::remove_if;

using boost::xpressive::sregex;
using boost::xpressive::smatch;
using boost::xpressive::regex_match;
using boost::lexical_cast;

time_t timestamp_parse(const string& input)
{
    string str = R"(
    (\d{4})
    (?:
        [^\d]?
        (\d{2})
        (?:
            [^\d]?
            (\d{2})
        )?
    )?
    [^\d]?
    (?:
        (\d{2})
        (?:
            [^\d]?
            (\d{2})
            (?:
                [^\d]?
                (\d{2})
            )?
        )?
    )?
    )";
    str.erase(
        remove_if(str.begin(), str.end(), ::isspace),
        str.end() );

    sregex re = sregex::compile(str);

    tm t;
    t.tm_mon = 11;
    t.tm_mday = 31;
    t.tm_hour = 23;
    t.tm_min = 59;
    t.tm_sec = 59;

    t.tm_isdst = -1;

    time_t ret(0);
    smatch what;
    if(regex_match(input, what, re))
    {
        t.tm_year = lexical_cast<int>(what[1]) - 1900;
        if (what[2] != "")
        {
            t.tm_mon = lexical_cast<int>(what[2]) - 1;
            if (what[3] != "")
            {
                t.tm_mday = lexical_cast<int>(what[3]);
                if (what[4] != "")
                {
                    t.tm_hour = lexical_cast<int>(what[4]);
                    if (what[5] != "")
                    {
                        t.tm_min = lexical_cast<int>(what[5]);
                        if (what[6] != "")
                        {
                            t.tm_sec = lexical_cast<int>(what[6]);
                        }
                    }
                }
            }
        }
        ret = mktime(&t);
    }
    if (ret == -1)
    {
        ret = time_t(0);
    }
    return ret;
}

} // namespace clas12::ccdb
} // namespace clas12

#endif // CLAS12_CCDB_TIMESTAMP_PARSE_HPP
