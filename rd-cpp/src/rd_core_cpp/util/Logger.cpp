#include "Logger.h"

#include "core_util.h"

#include <iostream>
#include <thread>

/*SwitchLogger::SwitchLogger(const std::string &category) {}

SwitchLogger get_logger(std::string category) {
    return SwitchLogger(category);
}*/

/*void SwitchLogger::log(LogLevel level, std::string message, std::exception const &e) {
    realLogger.log(level, message, e);
}

bool SwitchLogger::is_enabled(LogLevel level) {
    return realLogger.isEnabled(level);
}*/

namespace rd {
	string_view to_string(LogLevel level) {
		switch (level) {
			case LogLevel::Trace:
				return "Trace";
			case LogLevel::Debug:
				return "Debug";
			case LogLevel::Info:
				return "Info";
			case LogLevel::Warn:
				return "Warn";
			case LogLevel::Error:
				return "Error";
			case LogLevel::Fatal:
				return "Fatal";
		}
		return {};
	}

	void Logger::log(LogLevel level, string_view message, const std::exception *e) const {
		std::cerr << to_string(level)
				  << " | "
				  << to_string(std::this_thread::get_id())
				  << " | "
				  << message
				  << " | "
				  << (e ? e->what() : "")
				  << std::endl;
	}

	void Logger::trace(string_view msg, const std::exception *e) const {
		log(LogLevel::Trace, msg, e);
	}

	void Logger::debug(string_view msg, const std::exception *e) const {
		log(LogLevel::Debug, msg, e);
	}

	void Logger::info(string_view msg, const std::exception *e) const {
		log(LogLevel::Info, msg, e);
	}

	void Logger::warn(string_view msg, const std::exception *e) const {
		log(LogLevel::Warn, msg, e);
	}

	void Logger::error(string_view msg, const std::exception *e) const {
		log(LogLevel::Error, msg, e);
	}
}
