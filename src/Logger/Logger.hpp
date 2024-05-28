
#ifndef LOGGER_HPP
#define LOGGER_HPP

#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <ctime>

enum LogLevel : uint8_t
{
    LOG_LEVEL_CLEAN   = 1U << 0U,  
    LOG_LEVEL_FATAL   = 1U << 1U,  
    LOG_LEVEL_ERROR   = 1U << 2U,  
    LOG_LEVEL_WARNING = 1U << 3U,  
    LOG_LEVEL_INFO    = 1U << 4U,
    LOG_LEVEL_DEBUG   = 1U << 5U,  
    LOG_LEVEL_VERBOSE = 1U << 6U,
};

static std::string
convertLogLevel(LogLevel logLevel)
{
    switch (logLevel)
    {
    case LOG_LEVEL_VERBOSE:
        return "Verbose";
    case LOG_LEVEL_DEBUG:
        return "Debug";
    case LOG_LEVEL_INFO:
        return "Info";
    case LOG_LEVEL_WARNING:
        return "Warning";
    case LOG_LEVEL_ERROR:
        return "Error";
    case LOG_LEVEL_FATAL:
        return "Fatal";
    default:
        return "";
    }
}

static std::string
GetTimestamp()
{
    std::ostringstream oss;

    auto timepoint            = std::time(nullptr);
    auto timepointInLocalTime = *std::localtime(&timepoint);

    oss << std::put_time(&timepointInLocalTime, "%d-%m-%Y %H:%M:%S");

    return oss.str();
}

static bool IsLogLevelEnabled(LogLevel currentLevel, LogLevel checkLevel)
{
    return (currentLevel & checkLevel) != 0;
}

#define LOG(log_level) \
    if (IsLogLevelEnabled(currentLogLevel, log_level)) \
        std::cout << "Example Framework | " << GetTimestamp() << " | " << LOG_MODULE_NAME << " | " << convertLogLevel(log_level) << " | "

#define LOG_VERBOSE LOG(LogLevel::LOG_LEVEL_VERBOSE)
#define LOG_DEBUG   LOG(LogLevel::LOG_LEVEL_DEBUG)
#define LOG_INFO    LOG(LogLevel::LOG_LEVEL_INFO)
#define LOG_WARNING LOG(LogLevel::LOG_LEVEL_WARNING)
#define LOG_ERROR   LOG(LogLevel::LOG_LEVEL_ERROR)
#define LOG_FATAL   LOG(LogLevel::LOG_LEVEL_FATAL)

#define LOG_NOOP std::endl

#define SET_LOG_MODULE_NAME(Name) \
    static constexpr char const* LOG_MODULE_NAME = Name;
#define SET_LOG_LEVEL(Level) \
    static LogLevel currentLogLevel = Level;

#endif
