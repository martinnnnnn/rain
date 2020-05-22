#pragma once


#include <cstdio>
#include <cstring>
#include <cstdarg>
#include <mutex>
#include <functional>

#include "core/types.h"

namespace rain::core
{
    class logger
    {
    public:
		logger(std::function<void(const char*)> os);

        inline void clean_buffers()
        {
            memset(message_buffer, 0, sizeof(message_buffer));
            memset(header_buffer, 0, sizeof(header_buffer));
            memset(footer_buffer, 0, sizeof(footer_buffer));
        }

        void log_raw(const char* _format, ...);
        void log_level(const char* _level, const char* _format, ...);
        void log_max(const char* _filePath, u32 _fileLine, const char* _level, const char* _format, ...);
        void log_crash(const char* _filePath, u32 _fileLine, const char* _level, const char* _format, ...);
        void print_log();

        void print_log_raw();
               

    private:
        char message_buffer[1024 * 4];
        char header_buffer[1024];
        char footer_buffer[1024];
        std::mutex mut;

		std::function<void(const char*)> output_stream;
    };
}