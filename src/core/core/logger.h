#pragma once


#include <cstdio>
#include <cstring>
#include <cstdarg>

#include "core/types.h"

namespace rain::core
{
    class Logger
    {
    public:
        Logger(void(*_printOutput)(const char*));
        static constexpr char* LOG_DEBUG = "[DEBUG]";
        static constexpr char* LOG_INFO = "[INFO]";
        static constexpr char* LOG_NETWORK = "[NETWORK]";
        static constexpr char* LOG_PROFILE = "[PROFILE]";
        static constexpr char* LOG_WARNING = "[WARNING]";
        static constexpr char* LOG_ERROR = "[ERROR]";

        inline void clean_buffers()
        {
            memset(message_buffer, 0, sizeof(message_buffer));
            memset(header_buffer, 0, sizeof(header_buffer));
        }

        void log_raw(const char* _format, ...);
        void log_level(const char* _level, const char* _format, ...);
        void log_max(const char* _filePath, u32 _fileLine, const char* _level, const char* _format, ...);
        void print_log();

        void print_log_raw();
               

    private:
        char message_buffer[1024 * 4];
        char header_buffer[1024];
        void (*printOutput) (const char*);
    };
}