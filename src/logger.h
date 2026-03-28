#pragma once

#include <string>

enum class LogLevel {
    Info,
    Warning,
    Error,
    Debug
};

namespace Logger
{
    // Initializes the logger and clears the old log file
    void init(const std::string& filename);

    // Writes a formatted message to the log file
    void log(LogLevel level, const char* format, ...);
}