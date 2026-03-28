#include "logger.h"
#include <fstream>
#include <cstdarg>
#include <ctime>
#include <mutex>

namespace Logger
{
    static std::string logFile;
    static std::mutex logMutex;

    void init(const std::string& filename)
    {
        logFile = filename;
        std::ofstream ofs(logFile, std::ios::trunc); 
    }

    void log(LogLevel level, const char* format, ...)
    {
        if (logFile.empty()) return;

        std::lock_guard<std::mutex> lock(logMutex);

        std::time_t now = std::time(nullptr);
        char timeBuf[64];
        std::strftime(timeBuf, sizeof(timeBuf), "%Y-%m-%d %H:%M:%S", std::localtime(&now));

        const char* levelStr = "INFO ";
        switch (level)
        {
            case LogLevel::Info:    levelStr = "INFO "; break;
            case LogLevel::Warning: levelStr = "WARN "; break;
            case LogLevel::Error:   levelStr = "ERROR"; break;
            case LogLevel::Debug:   levelStr = "DEBUG"; break;
        }

        char msgBuf[1024];
        va_list args;
        va_start(args, format);
        vsnprintf(msgBuf, sizeof(msgBuf), format, args);
        va_end(args);

        std::ofstream ofs(logFile, std::ios::app);
        if (ofs.is_open())
        {
            ofs << "[" << timeBuf << "] [" << levelStr << "] " << msgBuf << "\n";
        }
    }
}