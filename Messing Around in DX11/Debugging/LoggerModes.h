#pragma once


#include <mutex>
#include <unordered_map>
#include <string>


#include <ostream>
#include <sstream>

#include "LoggingEnums.h"

using LogLevels = Logging::LogLevels;
using logger_config = std::unordered_map<std::string, std::string>;

namespace Logging
{
class Logger
{
public:
	Logger() = delete;
	// A Generic Logger, can be used as a Null Logger
	Logger(const LogLevels& max_log_level, const logger_config& config);
	virtual ~Logger() {};

	// Because there are now flushable stuff
	virtual bool canFlush() { return false; }
	virtual bool flush() { return false; } // Send the buffer to wherever it's wanted

	virtual bool log(const std::string& message, const LogLevels& level, const std::string& location = "");
protected:
	std::mutex lock;
	int m_MaxLogLevel;

	void changeLogLevel(const LogLevels& max_log_level)
	{
		m_MaxLogLevel = (int)max_log_level;
	}
private:
};

class BufferedLogger : public Logger
	{
	public:
		BufferedLogger() = delete;
		BufferedLogger(const LogLevels& max_log_level, const logger_config& config) : Logger(max_log_level, config), output(&buffer) {};
		bool canFlush() override { return true; }
		bool flush() { buffer.swap(std::stringbuf()); return true; }

		bool log(const std::string& message, const LogLevels& log_level, const std::string& location = "");
		
	protected:

		std::stringbuf buffer;
		std::ostream output;
	private:
	};



}// namespace Logging