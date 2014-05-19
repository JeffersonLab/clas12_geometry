#include "log.hpp"

#include <memory>
#include <iomanip>

#include <boost/log/core.hpp>
#include <boost/log/expressions.hpp>

#include <boost/log/utility/setup.hpp>

#include <boost/log/expressions/formatters/date_time.hpp>
#include <boost/log/support/date_time.hpp>

#include <boost/log/expressions/formatters/if.hpp>
#include <boost/log/expressions/predicates/is_in_range.hpp>

namespace clas12
{
namespace logging
{

    namespace expr = log::expressions;
    using log::core;

    static const auto fmt = expr::stream
        << expr::format_date_time<boost::posix_time::ptime>(
            "TimeStamp", "%Y-%m-%d %H:%M:%S")
        << " "
        << "[" << severity << "]"
        << expr::if_ (expr::is_in_range(severity,2,3))
        [
            expr::stream << "   "
        ]
        .else_
        [
            expr::stream << expr::if_ (expr::is_in_range(severity,3,4))
            [
                expr::stream << ""
            ]
            .else_
            [
                expr::stream << "  "
            ]
        ]
        << " "
        << expr::smessage;


    void remove_all_sinks()
    {
        core::get()->remove_all_sinks();
    }

    void add_file_log(const std::string& filename)
    {
        log::register_simple_formatter_factory<severity_level, char>("Severity");
        log::add_file_log(
            filename,
            log::keywords::format = fmt
        );
        log::add_common_attributes();
    }

    void add_console_log(std::ostream& os)
    {
        log::register_simple_formatter_factory<severity_level, char>("Severity");
        log::add_console_log(
            os,
            log::keywords::format = fmt
        );
        log::add_common_attributes();
    }

    void minimum_severity(severity_level min)
    {
        core::get()->set_filter(severity >= min);
    }
}
}
