#pragma once




#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <shellapi.h>

#if defined(min)
#undef min
#endif

#if defined(max)
#undef max
#endif

#include <string>

namespace rain::engine
{
    namespace Win32
    {
        std::string get_exe_path();
    }
}