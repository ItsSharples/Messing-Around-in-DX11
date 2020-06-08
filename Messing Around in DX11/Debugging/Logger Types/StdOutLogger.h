#pragma once

#include "../Logging.h"

#define LOGGER_STDOUT

namespace Logging 
{
	class Logger; // The Default Logger

	class StdOutLogger : public Logger
	{
	public:
		StdOutLogger() = delete;
		StdOutLogger(const LogLevels& max_log_level, const logger_config& config) : Logger(max_log_level, config) {};

		bool log(const std::string& message, const LogLevels& log_level, const std::string& location = "");
	protected:
	private:
	};

}// namespace Logging