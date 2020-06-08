#pragma once

#include "../Logging.h"

#define LOGGER_BUF_STDOUT

namespace Logging
{
class BufferedLogger; // The Default Logger

class BufferedStdLogger : public BufferedLogger
	{
	public:
		BufferedStdLogger() = delete;
		BufferedStdLogger(const LogLevels& max_log_level, const logger_config& config) : BufferedLogger(max_log_level, config) {};

		bool flush();
	protected:


	private:
	};


}// namespace Logging

