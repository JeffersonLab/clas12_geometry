#ifndef CLAS12_VERBOSITY_HPP
#define CLAS12_VERBOSITY_HPP

//#include <iostream>

namespace clas12
{

enum class Verbosity
{
    MUTE    = 0,
    QUIET   = 1,
    NORMAL  = 2,
    VERBOSE = 3,
    DEBUG   = 4,

    NONE               = MUTE,
    ONCE_PER_EXECUTION = QUIET,
    ONCE_PER_GROUP     = NORMAL,
    ONCE_PER_ITEM      = VERBOSE,
    MANY_PER_ITEM      = DEBUG
};

enum class Severity
{
    DEBUG = 0,
    INFO  = 1,
    WARN  = 2,
    ERROR = 3,
    FATAL = 4
};

struct LogLevel
{
    Verbosity verbosity;
    Severity severity;

    LogLevel(Verbosity verb=Verbosity::NORMAL, Severity sevr=Severity::ERROR)
    : verbosity(verb)
    , severity(sevr)
    {}

    bool operator<(const LogLevel& rhs)
    {
        if (verbosity < rhs.verbosity)
        {
            if (severity > rhs.severity)
            {
                return true;
            }
        }
        return false;
    }
};

} // namespace clas12

#endif // CLAS12_VERBOSITY_HPP
