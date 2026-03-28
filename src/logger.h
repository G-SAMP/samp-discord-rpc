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
    void init(const std::string& filename); // for initializing the logger file
    void log(LogLevel level, const char* format, ...);
}