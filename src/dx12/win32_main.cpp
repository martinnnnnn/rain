

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <shellapi.h> // For CommandLineToArgvW

#include <Shlwapi.h>

#include "win32_application.h"
#include "win32_tutorial.h"

#include "opengl/sandbox.h"
#include "dx12/win32_functional.h"
#include <dxgidebug.h>

enum class GFX_API
{
    OPENGL,
    DIRECTX12
};

GFX_API api = GFX_API::DIRECTX12;

//void StartOpenGL();
//void StartDirectX12(HINSTANCE hInstance);
//void ReportLiveObjects();
//
//int CALLBACK wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR lpCmdLine, int nCmdShow)
//{
//    switch (api)
//    {
//    case GFX_API::OPENGL:
//        StartOpenGL();
//        break;
//
//    case GFX_API::DIRECTX12:
//        StartDirectX12(hInstance);
//        break;
//    }
//
//    return 0;
//}
//
//void StartOpenGL()
//{
//    int argc;
//    wchar_t** argv = ::CommandLineToArgvW(::GetCommandLineW(), &argc);
//
//    runEngine(argc, (char**)argv);
//}


void ReportLiveObjects()
{
    IDXGIDebug1* dxgiDebug;
    DXGIGetDebugInterface1(0, IID_PPV_ARGS(&dxgiDebug));

    dxgiDebug->ReportLiveObjects(DXGI_DEBUG_ALL, DXGI_DEBUG_RLO_IGNORE_INTERNAL);
    dxgiDebug->Release();
}

int CALLBACK wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR lpCmdLine, int nCmdShow)
{

   // StartDirectX12(hInstance);

    int retCode = 0;

    // Set the working directory to the path of the executable.
    WCHAR path[MAX_PATH];
    HMODULE hModule = GetModuleHandleW(NULL);
    if (GetModuleFileNameW(hModule, path, MAX_PATH) > 0)
    {
        PathRemoveFileSpecW(path);
        SetCurrentDirectoryW(path);
    }

    Application::Create(hInstance);
    {
        Tutorial2* demo = new Tutorial2(L"Learning DirectX 12 - Lesson 2", 1280, 720);
        //std::shared_ptr<Tutorial2> demo = std::make_shared<Tutorial2>(L"Learning DirectX 12 - Lesson 2", 1280, 720);
        retCode = Application::Get().Run(demo);
    }
    Application::Destroy();

    atexit(&ReportLiveObjects);

    return retCode;
}