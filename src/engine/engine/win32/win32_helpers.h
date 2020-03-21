#pragma once


//#undef UNICODE
//#define UNICODE
//#undef _WINSOCKAPI_
//#define _WINSOCKAPI_

//#include <winsock2.h>
//#include <shellapi.h>
//#include <io.h>
//#include <ws2tcpip.h>
//#include <strsafe.h>
//#include <stdlib.h>
//#include <stdio.h>

#ifdef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#define DIRECTINPUT_VERSION 0x0800
#include <winsock2.h>
#include <Windows.h>
#include <windows.h>
#include <dinput.h>
#include <ws2tcpip.h>
#include <strsafe.h>
#include <stdlib.h>
#include <stdio.h>

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

        //void create_new_console(bool bindStdIn, bool bindStdOut, bool bindStdErr);
    }
}