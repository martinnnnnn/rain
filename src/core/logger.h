#pragma once


#include <cstdio>
#include <cstring>
#include <cstdarg>

#include "core/string.h"
#include "core/types.h"
#include "win32/win32_helper.h"
#include "win32/win32_application.h"

namespace rain
{
    class Logger
    {
    public:

        static constexpr char* LOG_DEBUG = "[DEBUG]";
        static constexpr char* LOG_INFO = "[INFO]";
        static constexpr char* LOG_WARNING = "[WARNING]";
        static constexpr char* LOG_ERROR = "[ERROR]";

        void log(const char* _format, ...);
        void log(const char* _level, const char* _format, ...);
        void log(const char* _filePath, u32 _fileLine, const char* _level, const char* _format, ...);
        void print_log();
               
        void create_new_console(bool bindStdIn, bool bindStdOut, bool bindStdErr);

    private:
        char message_buffer[1024 * 4];
        char header_buffer[1024];

    };
}

#ifdef _DEBUG
#define RAIN_LOG(...) do { rain::Application::get().logger->log(Logger::LOG_INFO, __VA_ARGS__); } while(false)
#define RAIN_LOG_DEBUG(...) do { rain::Application::get().logger->log(Logger::LOG_DEBUG, __VA_ARGS__); } while(false)
#define RAIN_LOG_WARNING(...) do { rain::Application::get().logger->log(__FILE__, __LINE__, Logger::LOG_WARNING, __VA_ARGS__); } while(false)
#define RAIN_LOG_ERROR(...) do { rain::Application::get().logger->log(__FILE__, __LINE__, Logger::LOG_ERROR, __VA_ARGS__); } while(false)
#else
#define RAIN_LOG(...)
#define RAIN_LOG_DEBUG(...)
#define RAIN_LOG_WARNING(...)
#define RAIN_LOG_ERROR(...)
#endif