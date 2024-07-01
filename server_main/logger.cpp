#include "logger.hpp"

#include <ctime>
#include <sstream>
#include <iomanip>

// TODO: Fix me

std::string getCurrentTime() {
    auto now = std::time(nullptr);
    std::ostringstream timeStream;
    timeStream << std::put_time(std::localtime(&now), "%Y-%m-%d %H:%M:%S");
    return timeStream.str();
}

// end TODO

Logger::Logger(const std::string& context) {
    _context = context;
}

Logger::~Logger() {}

void Logger::log(const std::string& message, const std::string& logLevel) {
    std::string timeStr = getCurrentTime();
    std::string logMessage = "[" + timeStr + "]" + "[" + logLevel + "]" + "[" + _context + "]" + ": " + message;

    std::cout << logMessage << std::endl;
}

void Logger::info(const std::string& message) {
    log(message, "INFO");
}

void Logger::error(const std::string& message) {
    log(message, "ERROR");
}