#include "log.hpp"

BOOST_LOG_GLOBAL_LOGGER_INIT(global_log, logger_type)
{
    logger_type lg;
    lg.add_attribute("StopWatch", boost::make_shared< attrs::timer >());
    return lg;
}
