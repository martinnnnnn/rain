#include "logger.h"


#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <io.h>
#include <fcntl.h>
#include <iostream>

#include "string/string.h"

namespace rain::core
{
    void Logger::init(void(*_printOutput)(const char*))
    {
        printOutput = _printOutput;
    }

    void Logger::clean_buffers()
    {
        memset(message_buffer, 0, sizeof(message_buffer));
        memset(header_buffer, 0, sizeof(header_buffer));
    }

    void Logger::log_level(const char* _level, const char* _format, ...)
    {
        String::print_to_buffer(header_buffer, sizeof(header_buffer), "%s ", _level);
        va_list args;
        va_start(args, _format);
        String::print_to_buffer_va(message_buffer, sizeof(message_buffer), _format, args);
        va_end(args);
        print_log();
    }

    void Logger::log_max(const char* _filePath, u32 _fileLine, const char* _level, const char* _format, ...)
    {
        String::print_to_buffer(header_buffer, sizeof(header_buffer), "%s Error at %s on line %u :\n", _level, _filePath, _fileLine);
        va_list args;
        va_start(args, _format);
        String::print_to_buffer_va(message_buffer, sizeof(message_buffer), _format, args);
        va_end(args);
        print_log();
    }

    void Logger::print_log()
    {
        printOutput(header_buffer);
        printOutput(message_buffer);
        printOutput("\n");
    }

    void Logger::log_raw(const char* _format, ...)
    {
        va_list args;
        va_start(args, _format);
        String::print_to_buffer_va(message_buffer, sizeof(message_buffer), _format, args);
        va_end(args);
        print_log_raw();
    }

    void Logger::print_log_raw()
    {
        printOutput(message_buffer);
    }

    

}