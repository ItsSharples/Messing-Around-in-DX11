#pragma once

#include <mutex>
#include <unordered_map>
#include <string>

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

}// namespace Logging