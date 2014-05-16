#ifndef CLAS12_LOG_HPP
#define CLAS12_LOG_HPP

#include <boost/log/sources/severity_logger.hpp>
#include <boost/log/sources/global_logger_storage.hpp>

enum severity_level
{
    debug = 0,
    info  = 1,
    warn  = 2,
    error = 3,
    fatal = 4
}

typedef boost::log::sources::severity_logger_mt<severity_level> logger_type;

BOOST_LOG_GLOBAL_LOGGER(global_log, logger_type)

#define LOG(level) BOOST_LOG_SEV(global_log::get(), level)

#endif // CLAS12_LOG_HPP
