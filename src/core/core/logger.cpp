#include "logger.h"

#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <io.h>
#include <fcntl.h>
#include <iostream>

#include "core/string.h"

namespace rain::core
{
    logger::logger(void(*_printOutput)(const char*))
    {
        printOutput = _printOutput;
        clean_buffers();
    }

    void logger::log_level(const char* _level, const char* _format, ...)
    {
        string::print_to_buffer(header_buffer, sizeof(header_buffer), "%s ", _level);
        va_list args;
        va_start(args, _format);
        string::print_to_buffer_va(message_buffer, sizeof(message_buffer), _format, args);
        va_end(args);
        print_log();
    }

    void logger::log_max(const char* _filePath, u32 _fileLine, const char* _level, const char* _format, ...)
    {
        string::print_to_buffer(header_buffer, sizeof(header_buffer), "%s Error at %s on line %u :\n", _level, _filePath, _fileLine);
        va_list args;
        va_start(args, _format);
        string::print_to_buffer_va(message_buffer, sizeof(message_buffer), _format, args);
        va_end(args);
        print_log();
    }

    void logger::print_log()
    {
        printOutput(header_buffer);
        printOutput(message_buffer);
        printOutput("\n");
    }

    void logger::log_raw(const char* _format, ...)
    {
        va_list args;
        va_start(args, _format);
        string::print_to_buffer_va(message_buffer, sizeof(message_buffer), _format, args);
        va_end(args);
        print_log_raw();
    }

    void logger::print_log_raw()
    {
        printOutput(message_buffer);
    }
}