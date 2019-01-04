#pragma once


#include <cstdio>
#include <cstring>
#include <cstdarg>

#include "core/core.h"
#include "engine/win32/win32_helpers.h"
#include "engine/win32/win32_application.h"

namespace rain::engine
{
    class Logger
    {
    public:

        static constexpr char* LOG_DEBUG = "[DEBUG]";
        static constexpr char* LOG_INFO = "[INFO]";
        static constexpr char* LOG_NETWORK = "[NETWORK]";
        static constexpr char* LOG_PROFILE = "[PROFILE]";
        static constexpr char* LOG_WARNING = "[WARNING]";
        static constexpr char* LOG_ERROR = "[ERROR]";

        void clean_buffers()
        {
            memset(message_buffer, 0, sizeof(message_buffer));
            memset(header_buffer, 0, sizeof(header_buffer));
        }

        void log_raw(const char* _format, ...);
        void log_level(const char* _level, const char* _format, ...);
        void log_max(const char* _filePath, u32 _fileLine, const char* _level, const char* _format, ...);
        void print_log();

        void print_log_raw();
               
        void create_new_console(bool bindStdIn, bool bindStdOut, bool bindStdErr);

    private:
        char message_buffer[1024 * 4];
        char header_buffer[1024];

    };
}

//#ifdef _DEBUG
//#define RAIN_LOG(...) do { rain::engine::Application::get()->logger->log_level(Logger::LOG_INFO, __VA_ARGS__); } while(false)
//#define RAIN_LOG_DEBUG(...) do { rain::engine::Application::get()->logger->log_level(Logger::LOG_DEBUG, __VA_ARGS__); } while(false)
//#define RAIN_LOG_NETWORK(...) do { rain::engine::Application::get()->logger->log_level(Logger::LOG_NETWORK, __VA_ARGS__); } while(false)
//#define RAIN_LOG_PROFILE(...) do { rain::engine::Application::get()->logger->log_level(Logger::LOG_PROFILE, __VA_ARGS__); } while(false)
//#define RAIN_LOG_WARNING(...) do { rain::engine::Application::get()->logger->log_max(__FILE__, __LINE__, Logger::LOG_WARNING, __VA_ARGS__); } while(false)
//#define RAIN_LOG_ERROR(...) do { rain::engine::Application::get()->logger->log_max(__FILE__, __LINE__, Logger::LOG_ERROR, __VA_ARGS__); } while(false)
//#define RAIN_LOG_RAW(...) do { rain::engine::Application::get()->logger->log_raw(__VA_ARGS__); } while(false)
//#else
//#define RAIN_LOG(...)
//#define RAIN_LOG_DEBUG(...)
//#define RAIN_LOG_WARNING(...)
//#define RAIN_LOG_ERROR(...)
//#endif