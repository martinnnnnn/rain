#include "win32_helpers.h"


#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "AdvApi32.lib")

#include "core/core.h"

#include <fcntl.h>
#include <iostream>

namespace rain::engine
{
    namespace Win32
    {
        std::string get_exe_path()
        {
            char buffer[MAX_PATH];
            GetModuleFileName(NULL, buffer, MAX_PATH);
            std::string path = std::string(buffer).substr(0, std::string(buffer).find_last_of("\\/"));
            core::string::replace(path, "\\", "/");
            return path;
        }

        //void create_new_console(bool bindStdIn, bool bindStdOut, bool bindStdErr)
        //{
        //    AllocConsole();
        //    if (bindStdIn)
        //    {
        //        FILE* dummyFile;
        //        freopen_s(&dummyFile, "nul", "r", stdin);
        //    }
        //    if (bindStdOut)
        //    {
        //        FILE* dummyFile;
        //        freopen_s(&dummyFile, "nul", "w", stdout);
        //    }
        //    if (bindStdErr)
        //    {
        //        FILE* dummyFile;
        //        freopen_s(&dummyFile, "nul", "w", stderr);
        //    }

        //    // Redirect unbuffered stdin from the current standard input handle
        //    if (bindStdIn)
        //    {
        //        HANDLE stdHandle = GetStdHandle(STD_INPUT_HANDLE);
        //        if (stdHandle != INVALID_HANDLE_VALUE)
        //        {
        //            int fileDescriptor = _open_osfhandle((intptr_t)stdHandle, _O_TEXT);
        //            if (fileDescriptor != -1)
        //            {
        //                FILE* file = _fdopen(fileDescriptor, "r");
        //                if (file != NULL)
        //                {
        //                    int dup2Result = _dup2(_fileno(file), _fileno(stdin));
        //                    if (dup2Result == 0)
        //                    {
        //                        setvbuf(stdin, NULL, _IONBF, 0);
        //                    }
        //                }
        //            }
        //        }
        //    }

        //    // Redirect unbuffered stdout to the current standard output handle
        //    if (bindStdOut)
        //    {
        //        HANDLE stdHandle = GetStdHandle(STD_OUTPUT_HANDLE);
        //        if (stdHandle != INVALID_HANDLE_VALUE)
        //        {
        //            int fileDescriptor = _open_osfhandle((intptr_t)stdHandle, _O_TEXT);
        //            if (fileDescriptor != -1)
        //            {
        //                FILE* file = _fdopen(fileDescriptor, "w");
        //                if (file != NULL)
        //                {
        //                    int dup2Result = _dup2(_fileno(file), _fileno(stdout));
        //                    if (dup2Result == 0)
        //                    {
        //                        setvbuf(stdout, NULL, _IONBF, 0);
        //                    }
        //                }
        //            }
        //        }
        //    }

        //    // Redirect unbuffered stderr to the current standard error handle
        //    if (bindStdErr)
        //    {
        //        HANDLE stdHandle = GetStdHandle(STD_ERROR_HANDLE);
        //        if (stdHandle != INVALID_HANDLE_VALUE)
        //        {
        //            int fileDescriptor = _open_osfhandle((intptr_t)stdHandle, _O_TEXT);
        //            if (fileDescriptor != -1)
        //            {
        //                FILE* file = _fdopen(fileDescriptor, "w");
        //                if (file != NULL)
        //                {
        //                    int dup2Result = _dup2(_fileno(file), _fileno(stderr));
        //                    if (dup2Result == 0)
        //                    {
        //                        setvbuf(stderr, NULL, _IONBF, 0);
        //                    }
        //                }
        //            }
        //        }
        //    }

        //    if (bindStdIn)
        //    {
        //        std::wcin.clear();
        //        std::cin.clear();
        //    }
        //    if (bindStdOut)
        //    {
        //        std::wcout.clear();
        //        std::cout.clear();
        //    }
        //    if (bindStdErr)
        //    {
        //        std::wcerr.clear();
        //        std::cerr.clear();
        //    }
        //}
    }
}