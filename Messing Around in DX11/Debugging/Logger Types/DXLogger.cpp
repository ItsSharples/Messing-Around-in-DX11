#include "pch.h"
#include "DXLogger.h"

bool Logging::DXLogger::log(const std::string& message, const LogLevels& log_level, const std::string& location)
{
	if(!Logger::log(message, log_level, location))
    {
        return false;
    }

    // TODO Complete DXLogger
	return false;
}
