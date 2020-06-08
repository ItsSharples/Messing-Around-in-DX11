#pragma once

#include <unordered_map>

#undef ERROR

namespace Logging
{
// The available Levels that loggers can accept
enum class LogLevels : int
{
	TRACE = 0, //Trace
	DEBUG, // Debug
	INFO, // Info
	WARN, // Warn
	ERROR, // Error
	HELP = 99, // Help

	ALL = ERROR + 1, // All
	NONE = TRACE - 1, // None
};

static std::unordered_map<LogLevels, std::string> level_names
{
	{LogLevels::TRACE, "TRACE"},
	{LogLevels::DEBUG, "DEBUG"},
	{LogLevels::INFO, "INFO"},
	{LogLevels::WARN, "WARN"},
	{LogLevels::ERROR, "ERROR"},
	{LogLevels::HELP, "HELP"},

	{LogLevels::ALL, "ALL"},
	{LogLevels::NONE, "NONE"},
};
}// namespace Logging