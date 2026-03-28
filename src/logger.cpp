#include "logger.h"
#include <fstream>
#include <cstdarg>
#include <ctime>
#include <mutex>

namespace Logger
{
    static std::string logFile;
    static std::mutex logMutex; // Ensures thread safety if multiple threads log at once

    void init(const std::string& filename)
    {
        logFile = filename;
        // Open with std::ios::trunc to clear the log file on every game startup
        std::ofstream ofs(logFile, std::ios::trunc); 
    }

    void log(LogLevel level, const char* format, ...)
    {
        if (logFile.empty()) return;

        // Lock the mutex so threads don't overwrite each other's text
        std::lock_guard<std::mutex> lock(logMutex);

        // 1. Get current time
        std::time_t now = std::time(nullptr);
        char timeBuf[64];
        std::strftime(timeBuf, sizeof(timeBuf), "%Y-%m-%d %H:%M:%S", std::localtime(&now));

        // 2. Determine Log Level String
        const char* levelStr = "INFO ";
        switch (level)
        {
            case LogLevel::Info:    levelStr = "INFO "; break;
            case LogLevel::Warning: levelStr = "WARN "; break;
            case LogLevel::Error:   levelStr = "ERROR"; break;
            case LogLevel::Debug:   levelStr = "DEBUG"; break;
        }

        // 3. Format the user's message (like printf)
        char msgBuf[1024];
        va_list args;
        va_start(args, format);
        vsnprintf(msgBuf, sizeof(msgBuf), format, args);
        va_end(args);

        // 4. Write to file
        std::ofstream ofs(logFile, std::ios::app);
        if (ofs.is_open())
        {
            ofs << "[" << timeBuf << "] [" << levelStr << "] " << msgBuf << "\n";
        }
    }
}