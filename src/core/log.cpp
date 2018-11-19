#include "log.h"


#include <Windows.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <io.h>
#include <fcntl.h>
#include <iostream>


namespace rain
{
    void Log::init(bool bindStdIn, bool bindStdOut, bool bindStdErr)
    {
        if (bindStdIn)
        {
            FILE* dummyFile;
            freopen_s(&dummyFile, "nul", "r", stdin);
        }
        if (bindStdOut)
        {
            FILE* dummyFile;
            freopen_s(&dummyFile, "nul", "w", stdout);
        }
        if (bindStdErr)
        {
            FILE* dummyFile;
            freopen_s(&dummyFile, "nul", "w", stderr);
        }

        // Redirect unbuffered stdin from the current standard input handle
        if (bindStdIn)
        {
            HANDLE stdHandle = GetStdHandle(STD_INPUT_HANDLE);
            if (stdHandle != INVALID_HANDLE_VALUE)
            {
                int fileDescriptor = _open_osfhandle((intptr_t)stdHandle, _O_TEXT);
                if (fileDescriptor != -1)
                {
                    FILE* file = _fdopen(fileDescriptor, "r");
                    if (file != NULL)
                    {
                        int dup2Result = _dup2(_fileno(file), _fileno(stdin));
                        if (dup2Result == 0)
                        {
                            setvbuf(stdin, NULL, _IONBF, 0);
                        }
                    }
                }
            }
        }

        // Redirect unbuffered stdout to the current standard output handle
        if (bindStdOut)
        {
            HANDLE stdHandle = GetStdHandle(STD_OUTPUT_HANDLE);
            if (stdHandle != INVALID_HANDLE_VALUE)
            {
                int fileDescriptor = _open_osfhandle((intptr_t)stdHandle, _O_TEXT);
                if (fileDescriptor != -1)
                {
                    FILE* file = _fdopen(fileDescriptor, "w");
                    if (file != NULL)
                    {
                        int dup2Result = _dup2(_fileno(file), _fileno(stdout));
                        if (dup2Result == 0)
                        {
                            setvbuf(stdout, NULL, _IONBF, 0);
                        }
                    }
                }
            }
        }

        // Redirect unbuffered stderr to the current standard error handle
        if (bindStdErr)
        {
            HANDLE stdHandle = GetStdHandle(STD_ERROR_HANDLE);
            if (stdHandle != INVALID_HANDLE_VALUE)
            {
                int fileDescriptor = _open_osfhandle((intptr_t)stdHandle, _O_TEXT);
                if (fileDescriptor != -1)
                {
                    FILE* file = _fdopen(fileDescriptor, "w");
                    if (file != NULL)
                    {
                        int dup2Result = _dup2(_fileno(file), _fileno(stderr));
                        if (dup2Result == 0)
                        {
                            setvbuf(stderr, NULL, _IONBF, 0);
                        }
                    }
                }
            }
        }

        if (bindStdIn)
        {
            std::wcin.clear();
            std::cin.clear();
        }
        if (bindStdOut)
        {
            std::wcout.clear();
            std::cout.clear();
        }
        if (bindStdErr)
        {
            std::wcerr.clear();
            std::cerr.clear();
        }
    }
}