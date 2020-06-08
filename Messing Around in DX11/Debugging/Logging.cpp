// Heavily based on Kevin Kreiser's Logging.hpp
// Source: https://gist.github.com/kevinkreiser/39f2e39273c625d96790


#include "pch.h"
#include "Logging.h"

#include "LoggerModes.h"
#include "LoggerTypes.h"

namespace Logging
{
		Logger* LoggingFactory::Generate(const LogLevels& max_log_level, const logger_config& config) const
			// Create a Logger and return it
		{
			auto type = config.find("type");
			if (type == config.end())
			{
				// No Logger Type
				throw std::runtime_error("Cannot Generate a Logger without a Type");
			}
			auto logger = logging_types.find(type->second);
			if (logger == logging_types.end())
			{
				// Invalid Logger Type
				throw std::runtime_error("Unknown Logger Type: " + type->second);
			}
			return logger->second(max_log_level, config);
		}
		bool LoggingFactory::addNewLoggerType(const std::string& name, const generic_logger& logger)
		{
			if (logging_types.find(name) != logging_types.end())
			{
				// Already Exists
				return false;
			}

			logging_types.emplace(name, logger);
			return true;

		}
#define LoggerType(name, type) name, [](const LogLevels& max_log_level, const logger_config& config)->Logger* {return new type(max_log_level, config); } 
		LoggingFactory::LoggingFactory()
		{
			addNewLoggerType(LoggerType("", Logging::Logger));
#ifdef LOGGER_STDOUT
			addNewLoggerType(LoggerType("std out", Logging::StdOutLogger));
#endif // LOGGER_STDOUT
#ifdef LOGGER_BUF_STDOUT
			addNewLoggerType(LoggerType("buffered std out", Logging::BufferedStdLogger));
#endif // LOGGER_BUF_STDOUT
#undef LoggerType
		}
}// namespace Logger;