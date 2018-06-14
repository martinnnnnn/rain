#include "win32_output.h"

#include <windows.h>
#include <stdio.h>

namespace rain
{
    void win32_console_output(const char* buffer)
    {
        OutputDebugString(buffer);
    }
}


