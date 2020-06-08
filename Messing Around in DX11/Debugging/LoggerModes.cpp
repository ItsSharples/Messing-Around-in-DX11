#include "pch.h"
#include "LoggerModes.h"

#include "Timer.h"


// A Generic Logger, can be used as a Null Logger
Logging::Logger::Logger(const LogLevels& max_log_level, const logger_config& config)
{
	changeLogLevel(max_log_level);
	//std::cout << "Log Level set to: " + level_names.at(max_log_level) << std::endl;

	// Suppress Warnings
	max_log_level;
	config;
};

bool Logging::Logger::log(const std::string& message, const LogLevels& level, const std::string& location)
{
	return !((int)level > m_MaxLogLevel);
	// Suppress Warnings
	message; location;
};

bool Logging::BufferedLogger::log(const std::string& message, const LogLevels& log_level, const std::string& location)
{
    // Does checks that all loggers require
    if(!Logger::log(message, log_level, location))
    {
        return false;
    }
    if(!location.empty())
    {
        output << std::boolalpha
            << level_names.at(log_level) + "::" + Time::prettyPrintTime() +
            " at " + location + " says: \"" + message + "\""
            << std::endl;
    }
    else
    {
        output << std::boolalpha
            << level_names.at(log_level) + "::" + Time::prettyPrintTime() +
            ": " + message
            << std::endl;
    }
    return true;
}