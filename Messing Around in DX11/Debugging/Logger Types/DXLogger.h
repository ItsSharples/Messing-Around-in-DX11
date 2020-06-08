#pragma once

#include "../Logging.h"

#define LOGGER_DX

namespace Logging
{
	class Logger;

	class DXLogger :
		public Logger
	{
	public:
		DXLogger() = delete;
		DXLogger(const LogLevels& max_log_level, const logger_config& config) : Logger(max_log_level, config) {};

		bool log(const std::string& message, const LogLevels& log_level, const std::string& location = "");
	protected:
	private:
		//TODO Add RenderTarget

		//TODO Think of a better way to handle rendering to the screen?
	};

}// namespace Logging