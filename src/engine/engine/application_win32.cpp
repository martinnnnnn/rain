

#include "core/src/logger.h"

#include "helper_win32.h"


int WINAPI WinMain(HINSTANCE hinstance, HINSTANCE hprevinstance, LPSTR lpcmdline, int nshowcmd)
{


    OutputDebugString("hello");

    return 0;
}