#include "pch.h"
#include "StdOutLogger.h"


bool Logging::StdOutLogger::log(const std::string& message, const LogLevels& log_level,
    const std::string& location)
{
    // Does checks that all loggers require
    if(!Logger::log(message, log_level, location))
    {
        return false;
    }
    if(!location.empty())
    {
        std::cout << std::boolalpha
            << level_names.at(log_level) + "::" + Time::prettyPrintTime() +
            " at " + location + " says: \"" + message + "\""
            << std::endl;
    }
    else
    {
        std::cout << std::boolalpha
            << level_names.at(log_level) + "::" + Time::prettyPrintTime() +
            ": " + message
            << std::endl;
    }
    return true;
}
