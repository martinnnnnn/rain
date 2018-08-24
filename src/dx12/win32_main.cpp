

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <shellapi.h> // For CommandLineToArgvW

#include "opengl/sandbox.h"
#include "dx12/win32_functional.h"

enum class GFX_API
{
    OPENGL,
    DIRECTX12
};

GFX_API api = GFX_API::DIRECTX12;

void StartOpenGL();
void StartDirectX12(HINSTANCE hInstance);

int CALLBACK wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR lpCmdLine, int nCmdShow)
{
    switch (api)
    {
    case GFX_API::OPENGL:
        StartOpenGL();
        break;

    case GFX_API::DIRECTX12:
        StartDirectX12(hInstance);
        break;
    }

    return 0;
}

void StartOpenGL()
{
    int argc;
    wchar_t** argv = ::CommandLineToArgvW(::GetCommandLineW(), &argc);

    runEngine(argc, (char**)argv);
}

