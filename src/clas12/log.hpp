#ifndef CLAS12_LOG_HPP
#define CLAS12_LOG_HPP

#include <ostream>
#include <string>

#ifdef HAVE_BOOST_LOG

#include <boost/log/trivial.hpp>

#define LOG(x) BOOST_LOG_TRIVIAL(x)

namespace clas12
{
namespace logging
{
    /**
     * levels:
     *      0  trace
     *      1  debug
     *      2  info
     *      3  warning
     *      4  error
     *      5  fatal
     **/

    namespace log = boost::log;
    using namespace log::trivial;

    void remove_all_sinks();
    void add_file_log(const std::string& filename);
    void add_console_log(std::ostream& os);
    void minimum_severity(severity_level min);
}
}

#else // if not HAVE_BOOST_LOG

#define LOG(x) std::clog

namespace clas12
{
namespace logging
{
    void remove_all_sinks();
    void add_file_log(const std::string& filename);
    void add_console_log(std::ostream& os);
    void minimum_severity(severity_level min);
}
}


#endif

#endif // CLAS12_LOG_HPP
