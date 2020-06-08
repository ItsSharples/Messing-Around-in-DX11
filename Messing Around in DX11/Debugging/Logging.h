#pragma once


// Heavily based on Kevin Kreiser's Logging.hpp
// Source: https://gist.github.com/kevinkreiser/39f2e39273c625d96790

#include <string>
#include <unordered_map>
#include <mutex>
#include <iostream>


#include "Timer.h"

// Similar to max/min being defined in windows.h, some places define ERROR
#undef ERROR


#include "LoggerModes.h"
#include "LoggingEnums.h"


using logger_config = std::unordered_map<std::string, std::string>;

namespace Logging
{
	class Logger;
	class BufferedLogger;
	class LoggingFactory;

	// The available Levels that loggers can accept


	// Include LoggingFactory??


	// Magic Function Pointer for multiple implementations of Logger
	using generic_logger = Logger * (*)(const LogLevels&, const logger_config&); // Makes a generic version of a Logger so I can make an array of them

	using logger_map = std::unordered_map<std::string, generic_logger >;
	using logging_pair = std::pair<std::string, generic_logger>;

	class LoggingFactory
	{
	public:
		Logger* Generate(const LogLevels& max_log_level, const logger_config& config) const;
		bool addNewLoggerType(const std::string& name, const generic_logger& logger);

	protected:
		logger_map getLoggingTypes() { return logging_types; }
	private:
		logger_map logging_types;

		LoggingFactory();

		friend inline LoggingFactory& getFactory();

	};

	//get the Logging Factory
	inline LoggingFactory& getFactory() {
		static LoggingFactory factory_singleton{};
		return factory_singleton;
	}

	//get at the singleton
	inline Logger& getCurrentLogger(const LogLevels& max_log_level = LogLevels::NONE, const logger_config& config = { {"type", "std out"}, {"color", ""} }) {
		static std::unique_ptr<Logger> singleton(getFactory().Generate(max_log_level, config));
		return *singleton;
	}


	// Magic Logging Bits and Bobs
	//inline void log(const std::string& message, const int& log_level) { getCurrentLogger().log(message, log_level); }
	inline void log(const std::string& message, const LogLevels& log_level, const std::string& location = "") { getCurrentLogger().log(message, log_level, location); }
#define source_log(message, log_level) log(message, log_level, __FUNCTION__)
	inline void TRACE(const std::string& message) { getCurrentLogger().log(message, LogLevels::TRACE); }
	inline void DEBUG(const std::string& message) { getCurrentLogger().log(message, LogLevels::DEBUG); }
	inline void INFO(const std::string& message) { getCurrentLogger().log(message, LogLevels::INFO); }
	inline void WARN(const std::string& message) { getCurrentLogger().log(message, LogLevels::WARN); }
	inline void ERROR(const std::string& message) { getCurrentLogger().log(message, LogLevels::ERROR); }
	inline void HELP(const std::string& message) { getCurrentLogger().log(message, LogLevels::HELP); }
	inline void Flush() { if(getCurrentLogger().canFlush()) { getCurrentLogger().flush(); } }

	// Helper Functions
	static inline void StartLogging(const std::string& log_type, const LogLevels& max_log_level)
	{
		getCurrentLogger(max_log_level, { {"type", log_type}, {"color",""} });
	}

}// namespace Logger;