//
//
//#define WIN32_LEAN_AND_MEAN
//#include <Windows.h>
//#include <shellapi.h> // For CommandLineToArgvW
//
//#include <Shlwapi.h>
//
//#include "win32_application.h"
//#include "win32_tutorial.h"
//
//#include "opengl/sandbox.h"
//#include "dx12/win32_functional.h"
//#include <dxgidebug.h>
//
//enum class GFX_API
//{
//    OPENGL,
//    DIRECTX12
//};
//
//GFX_API api = GFX_API::DIRECTX12;
//
//void StartOpenGL();
//void StartDirectX12Functional(HINSTANCE hInstance);
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
//        //StartDirectX12Functional(hInstance);
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
//
//void StartDirectX12(HINSTANCE hInstance)
//{
//    // StartDirectX12(hInstance);
//
//    int retCode = 0;
//
//    // Set the working directory to the path of the executable.
//    WCHAR path[MAX_PATH];
//    HMODULE hModule = GetModuleHandleW(NULL);
//    if (GetModuleFileNameW(hModule, path, MAX_PATH) > 0)
//    {
//        PathRemoveFileSpecW(path);
//        SetCurrentDirectoryW(path);
//    }
//
//    Application::Create(hInstance);
//    {
//        Tutorial2* demo = new Tutorial2(L"Learning DirectX 12 - Lesson 2", 1280, 720);
//        //std::shared_ptr<Tutorial2> demo = std::make_shared<Tutorial2>(L"Learning DirectX 12 - Lesson 2", 1280, 720);
//        retCode = Application::Get().Run(demo);
//    }
//    Application::Destroy();
//
//    atexit(&ReportLiveObjects);
//}
//
//void ReportLiveObjects()
//{
//    IDXGIDebug1* dxgiDebug;
//    DXGIGetDebugInterface1(0, IID_PPV_ARGS(&dxgiDebug));
//
//    dxgiDebug->ReportLiveObjects(DXGI_DEBUG_ALL, DXGI_DEBUG_RLO_IGNORE_INTERNAL);
//    dxgiDebug->Release();
//}
//
//void Resize(uint32_t width, uint32_t height)
//{
//    if (g_ClientWidth != width || g_ClientHeight != height)
//    {
//        // Don't allow 0 size swap chain back buffers.
//        g_ClientWidth = std::max(1u, width);
//        g_ClientHeight = std::max(1u, height);
//
//        // Flush the GPU queue to make sure the swap chain's back buffers
//        // are not being referenced by an in-flight command list.
//        Flush(g_CommandQueue, g_Fence, g_FenceValue, g_FenceEvent);
//
//        for (int i = 0; i < g_NumFrames; ++i)
//        {
//            // Any references to the back buffers must be released
//            // before the swap chain can be resized.
//            g_BackBuffers[i].Reset();
//            g_FrameFenceValues[i] = g_FrameFenceValues[g_CurrentBackBufferIndex];
//        }
//
//        DXGI_SWAP_CHAIN_DESC swapChainDesc = {};
//        ThrowIfFailed(g_SwapChain->GetDesc(&swapChainDesc));
//        ThrowIfFailed(g_SwapChain->ResizeBuffers(g_NumFrames, g_ClientWidth, g_ClientHeight,
//            swapChainDesc.BufferDesc.Format, swapChainDesc.Flags));
//
//        g_CurrentBackBufferIndex = g_SwapChain->GetCurrentBackBufferIndex();
//
//        UpdateRenderTargetViews(g_Device, g_SwapChain, g_RTVDescriptorHeap);
//    }
//}
////
////void SetFullscreen(bool fullscreen)
////{
////    if (g_Fullscreen != fullscreen)
////    {
////        g_Fullscreen = fullscreen;
////
////        if (g_Fullscreen) // Switching to fullscreen.
////        {
////            // Store the current window dimensions so they can be restored 
////            // when switching out of fullscreen state.
////            ::GetWindowRect(g_hWnd, &g_WindowRect);
////            // Set the window style to a borderless window so the client area fills
////            // the entire screen.
////            UINT windowStyle = WS_OVERLAPPEDWINDOW & ~(WS_CAPTION | WS_SYSMENU | WS_THICKFRAME | WS_MINIMIZEBOX | WS_MAXIMIZEBOX);
////
////            ::SetWindowLongW(g_hWnd, GWL_STYLE, windowStyle);
////
////            // Query the name of the nearest display device for the window.
////            // This is required to set the fullscreen dimensions of the window
////            // when using a multi-monitor setup.
////            HMONITOR hMonitor = ::MonitorFromWindow(g_hWnd, MONITOR_DEFAULTTONEAREST);
////            MONITORINFOEX monitorInfo = {};
////            monitorInfo.cbSize = sizeof(MONITORINFOEX);
////            ::GetMonitorInfo(hMonitor, &monitorInfo);
////            ::SetWindowPos(g_hWnd, HWND_TOP,
////                monitorInfo.rcMonitor.left,
////                monitorInfo.rcMonitor.top,
////                monitorInfo.rcMonitor.right - monitorInfo.rcMonitor.left,
////                monitorInfo.rcMonitor.bottom - monitorInfo.rcMonitor.top,
////                SWP_FRAMECHANGED | SWP_NOACTIVATE);
////
////            ::ShowWindow(g_hWnd, SW_MAXIMIZE);
////        }
////        else
////        {
////            // Restore all the window decorators.
////            ::SetWindowLong(g_hWnd, GWL_STYLE, WS_OVERLAPPEDWINDOW);
////
////            ::SetWindowPos(g_hWnd, HWND_NOTOPMOST,
////                g_WindowRect.left,
////                g_WindowRect.top,
////                g_WindowRect.right - g_WindowRect.left,
////                g_WindowRect.bottom - g_WindowRect.top,
////                SWP_FRAMECHANGED | SWP_NOACTIVATE);
////
////            ::ShowWindow(g_hWnd, SW_NORMAL);
////        }
////    }
////}
//
//LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
//
//LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
//{
//    if (g_IsInitialized)
//    {
//        switch (message)
//        {
//        case WM_PAINT:
//            Update();
//            Render();
//            break;
//        case WM_SYSKEYDOWN:
//        case WM_KEYDOWN:
//        {
//            bool alt = (::GetAsyncKeyState(VK_MENU) & 0x8000) != 0;
//
//            switch (wParam)
//            {
//            case 'V':
//                g_VSync = !g_VSync;
//                break;
//            case VK_ESCAPE:
//                ::PostQuitMessage(0);
//                break;
//            case VK_RETURN:
//                if (alt)
//                {
//            case VK_F11:
//                SetFullscreen(!g_Fullscreen);
//                }
//                break;
//            }
//        }
//        break;
//        // The default window procedure will play a system notification sound 
//        // when pressing the Alt+Enter keyboard combination if this message is 
//        // not handled.
//        case WM_SYSCHAR:
//            break;
//        case WM_SIZE:
//        {
//            RECT clientRect = {};
//            ::GetClientRect(g_hWnd, &clientRect);
//
//            int width = clientRect.right - clientRect.left;
//            int height = clientRect.bottom - clientRect.top;
//
//            Resize(width, height);
//        }
//        break;
//        case WM_DESTROY:
//            ::PostQuitMessage(0);
//            break;
//        default:
//            return ::DefWindowProcW(hwnd, message, wParam, lParam);
//        }
//    }
//    else
//    {
//        return ::DefWindowProcW(hwnd, message, wParam, lParam);
//    }
//
//    return 0;
//}
//
////void StartDirectX12Functional(HINSTANCE hInstance)
////{
////    // Windows 10 Creators update adds Per Monitor V2 DPI awareness context.
////    // Using this awareness context allows the client area of the window 
////    // to achieve 100% scaling while still allowing non-client window content to 
////    // be rendered in a DPI sensitive fashion.
////    SetThreadDpiAwarenessContext(DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE_V2);
////
////    // Window class name. Used for registering / creating the window.
////    const wchar_t* windowClassName = L"DX12WindowClass";
////    ParseCommandLineArguments();
////
////    EnableDebugLayer();
////
////    g_TearingSupported = CheckTearingSupport();
////
////    RegisterWindowClass(hInstance, windowClassName);
////    g_hWnd = CreateWindow(windowClassName, hInstance, L"Rain Engine -- DirectX 12",
////        g_ClientWidth, g_ClientHeight);
////
////    // Initialize the global window rect variable.
////    ::GetWindowRect(g_hWnd, &g_WindowRect);
////
////    ComPtr<IDXGIAdapter4> dxgiAdapter4 = GetAdapter(g_UseWarp);
////
////    g_Device = CreateDevice(dxgiAdapter4);
////
////    g_CommandQueue = CreateCommandQueue(g_Device, D3D12_COMMAND_LIST_TYPE_DIRECT);
////
////    g_SwapChain = CreateSwapChain(g_hWnd, g_CommandQueue,
////        g_ClientWidth, g_ClientHeight, g_NumFrames);
////
////    g_CurrentBackBufferIndex = g_SwapChain->GetCurrentBackBufferIndex();
////
////    g_RTVDescriptorHeap = CreateDescriptorHeap(g_Device, D3D12_DESCRIPTOR_HEAP_TYPE_RTV, g_NumFrames);
////    g_RTVDescriptorSize = g_Device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
////
////    UpdateRenderTargetViews(g_Device, g_SwapChain, g_RTVDescriptorHeap);
////
////    for (int i = 0; i < g_NumFrames; ++i)
////    {
////        g_CommandAllocators[i] = CreateCommandAllocator(g_Device, D3D12_COMMAND_LIST_TYPE_DIRECT);
////    }
////    g_CommandList = CreateCommandList(g_Device,
////        g_CommandAllocators[g_CurrentBackBufferIndex], D3D12_COMMAND_LIST_TYPE_DIRECT);
////
////    g_Fence = CreateFence(g_Device);
////    g_FenceEvent = CreateEventHandle();
////
////    g_IsInitialized = true;
////
////    ::ShowWindow(g_hWnd, SW_SHOW);
////
////    MSG msg = {};
////    while (msg.message != WM_QUIT)
////    {
////        if (::PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
////        {
////            ::TranslateMessage(&msg);
////            ::DispatchMessage(&msg);
////        }
////    }
////
////    // Make sure the command queue has finished all commands before closing.
////    Flush(g_CommandQueue, g_Fence, g_FenceValue, g_FenceEvent);
////
////    ::CloseHandle(g_FenceEvent);
////}
//
//void RegisterWindowClass(HINSTANCE hInst, const wchar_t* windowClassName)
//{
//    // Register a window class for creating our render window with.
//    WNDCLASSEXW windowClass = {};
//
//    windowClass.cbSize = sizeof(WNDCLASSEX);
//    windowClass.style = CS_HREDRAW | CS_VREDRAW;
//    windowClass.lpfnWndProc = &WndProc;
//    windowClass.cbClsExtra = 0;
//    windowClass.cbWndExtra = 0;
//    windowClass.hInstance = hInst;
//    windowClass.hIcon = ::LoadIcon(hInst, NULL);
//    windowClass.hCursor = ::LoadCursor(NULL, IDC_ARROW);
//    windowClass.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
//    windowClass.lpszMenuName = NULL;
//    windowClass.lpszClassName = windowClassName;
//    windowClass.hIconSm = ::LoadIcon(hInst, NULL);
//
//    static ATOM atom = ::RegisterClassExW(&windowClass);
//    assert(atom > 0);
//}
//
//HWND CreateWindow(const wchar_t* windowClassName, HINSTANCE hInst,
//    const wchar_t* windowTitle, uint32_t width, uint32_t height)
//{
//    int screenWidth = ::GetSystemMetrics(SM_CXSCREEN);
//    int screenHeight = ::GetSystemMetrics(SM_CYSCREEN);
//
//    RECT windowRect = { 0, 0, static_cast<LONG>(width), static_cast<LONG>(height) };
//    ::AdjustWindowRect(&windowRect, WS_OVERLAPPEDWINDOW, FALSE);
//
//    int windowWidth = windowRect.right - windowRect.left;
//    int windowHeight = windowRect.bottom - windowRect.top;
//
//    // Center the window within the screen. Clamp to 0, 0 for the top-left corner.
//    int windowX = std::max<int>(0, (screenWidth - windowWidth) / 2);
//    int windowY = std::max<int>(0, (screenHeight - windowHeight) / 2);
//    HWND hWnd = ::CreateWindowExW(
//        NULL,
//        windowClassName,
//        windowTitle,
//        WS_OVERLAPPEDWINDOW,
//        windowX,
//        windowY,
//        windowWidth,
//        windowHeight,
//        NULL,
//        NULL,
//        hInst,
//        nullptr
//    );
//
//    assert(hWnd && "Failed to create window");
//
//    return hWnd;
//}
//
//void StartDirectX12Functional(HINSTANCE hInstance)
//{
//    // Windows 10 Creators update adds Per Monitor V2 DPI awareness context.
//    // Using this awareness context allows the client area of the window 
//    // to achieve 100% scaling while still allowing non-client window content to 
//    // be rendered in a DPI sensitive fashion.
//    SetThreadDpiAwarenessContext(DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE_V2);
//
//    // Window class name. Used for registering / creating the window.
//    const wchar_t* windowClassName = L"DX12WindowClass";
//    ParseCommandLineArguments();
//
//    EnableDebugLayer();
//
//    g_TearingSupported = CheckTearingSupport();
//
//    RegisterWindowClass(hInstance, windowClassName);
//    g_hWnd = CreateWindow(windowClassName, hInstance, L"Rain Engine -- DirectX 12",
//        g_ClientWidth, g_ClientHeight);
//
//    // Initialize the global window rect variable.
//    ::GetWindowRect(g_hWnd, &g_WindowRect);
//
//    ComPtr<IDXGIAdapter4> dxgiAdapter4 = GetAdapter(g_UseWarp);
//
//    g_Device = CreateDevice(dxgiAdapter4);
//
//    g_CommandQueue = CreateCommandQueue(g_Device, D3D12_COMMAND_LIST_TYPE_DIRECT);
//
//    g_SwapChain = CreateSwapChain(g_hWnd, g_CommandQueue,
//        g_ClientWidth, g_ClientHeight, g_NumFrames);
//
//    g_CurrentBackBufferIndex = g_SwapChain->GetCurrentBackBufferIndex();
//
//    g_RTVDescriptorHeap = CreateDescriptorHeap(g_Device, D3D12_DESCRIPTOR_HEAP_TYPE_RTV, g_NumFrames);
//    g_RTVDescriptorSize = g_Device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
//
//    UpdateRenderTargetViews(g_Device, g_SwapChain, g_RTVDescriptorHeap);
//
//    for (int i = 0; i < g_NumFrames; ++i)
//    {
//        g_CommandAllocators[i] = CreateCommandAllocator(g_Device, D3D12_COMMAND_LIST_TYPE_DIRECT);
//    }
//    g_CommandList = CreateCommandList(g_Device,
//        g_CommandAllocators[g_CurrentBackBufferIndex], D3D12_COMMAND_LIST_TYPE_DIRECT);
//
//    g_Fence = CreateFence(g_Device);
//    g_FenceEvent = CreateEventHandle();
//
//    g_IsInitialized = true;
//
//    ::ShowWindow(g_hWnd, SW_SHOW);
//
//    MSG msg = {};
//    while (msg.message != WM_QUIT)
//    {
//        if (::PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
//        {
//            ::TranslateMessage(&msg);
//            ::DispatchMessage(&msg);
//        }
//    }
//
//    // Make sure the command queue has finished all commands before closing.
//    Flush(g_CommandQueue, g_Fence, g_FenceValue, g_FenceEvent);
//
//    ::CloseHandle(g_FenceEvent);
//}
//
//
//void ParseCommandLineArguments()
//{
//    int argc;
//    wchar_t** argv = ::CommandLineToArgvW(::GetCommandLineW(), &argc);
//
//    for (size_t i = 0; i < argc; ++i)
//    {
//        if (::wcscmp(argv[i], L"-w") == 0 || ::wcscmp(argv[i], L"--width") == 0)
//        {
//        }
//        if (::wcscmp(argv[i], L"-h") == 0 || ::wcscmp(argv[i], L"--height") == 0)
//        {
//        }
//        if (::wcscmp(argv[i], L"-warp") == 0 || ::wcscmp(argv[i], L"--warp") == 0)
//        {
//        }
//    }
//
//    // Free memory allocated by CommandLineToArgvW
//    ::LocalFree(argv);
//}



#include <windows.h>

#include <gl/gl.h>			// OpenGL headers
//#include <gl/glu.h>
#include <GL/freeglut.h>

#define WINDOWED		0					// predefined flags for initialization
#define FULLSCREEN		1

#define SCRWIDTH		640					// width of the window
#define SCRHEIGHT		480					// height of the window
#define SCRDEPTH		16					// bit depth

#define WNDCLASSNAME	"GLClass"			// window class name
#define WNDNAME			"OpenGL base code"	// string that will appear in the title bar


HDC hdc;
HWND hwnd;
HGLRC hglrc;

int screenw = SCRWIDTH;
int screenh = SCRHEIGHT;
int screenmode = 0;

bool quit = false;

int indexPixelFormat = 0;

void RenderFrame(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void SysRecoverDisplayMode(void)
{
    ChangeDisplaySettings(NULL, 0);
}


void SysShutdown(void)
{
    wglMakeCurrent(hdc, NULL);		// release device context in use by rc
    wglDeleteContext(hglrc);		// delete rendering context

    PostQuitMessage(0);				// make sure the window will be destroyed

    if (screenmode == FULLSCREEN)	// if FULLSCREEN, change back to original resolution
        SysRecoverDisplayMode();
}

void SysSetDisplayMode(int width, int height, int depth)
{
    DEVMODE dmode;

    memset(&dmode, 0, sizeof(DEVMODE));
    dmode.dmSize = sizeof(DEVMODE);
    dmode.dmPelsWidth = width;
    dmode.dmPelsHeight = height;
    dmode.dmBitsPerPel = depth;
    dmode.dmFields = DM_PELSWIDTH | DM_PELSHEIGHT | DM_BITSPERPEL;

    // change resolution, if possible
    if (ChangeDisplaySettings(&dmode, CDS_FULLSCREEN) != DISP_CHANGE_SUCCESSFUL)
    {
        // if not... failed to change resolution

        screenmode = 0;	// this prevents SysShutdown from changing resolution back

        MessageBox(NULL, "Your system doesn't support this screen resolution", "Display Error", MB_OK);
        SysShutdown();
    }
}








// CreateWnd: creates either a "full-screen" or a "windowed" window according to the flag - type
//            type = 0: windowed
//			  type = 1: full-screen

void CreateWnd(HINSTANCE &hinst, int width, int height, int depth, int type)
{
    // center position of the window
    int posx = (GetSystemMetrics(SM_CXSCREEN) / 2) - (width / 2);
    int posy = (GetSystemMetrics(SM_CYSCREEN) / 2) - (height / 2);

    // set up the window for a windowed application by default
    long wndStyle = WS_OVERLAPPEDWINDOW;
    screenmode = WINDOWED;

    if (type == FULLSCREEN)	// create a full-screen application if requested
    {
        wndStyle = WS_POPUP;
        screenmode = FULLSCREEN;

        posx = 0;
        posy = 0;

        // change resolution before the window is created
        SysSetDisplayMode(screenw, screenh, SCRDEPTH);
    }

    // create the window
    hwnd = CreateWindowEx(NULL,
        WNDCLASSNAME,
        WNDNAME,
        wndStyle | WS_CLIPCHILDREN | WS_CLIPSIBLINGS,
        posx, posy,
        width, height,
        NULL,
        NULL,
        hinst,
        NULL);

    // at this point WM_CREATE message is sent/received
    // the WM_CREATE branch inside WinProc function will execute here
}

void SetGLFormat(void)
{
    PIXELFORMATDESCRIPTOR pfd =
    {
        sizeof(PIXELFORMATDESCRIPTOR),
            1,
            PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,
            PFD_TYPE_RGBA,
            SCRDEPTH,
            0,0,0,0,0,0,0,0,0,0,0,0,0, // useles parameters
            16,
            0,0,PFD_MAIN_PLANE,0,0,0,0
    };

    // Choose the closest pixel format available
    if (!(indexPixelFormat = ChoosePixelFormat(hdc, &pfd)))
    {
        MessageBox(hwnd, "Failed to find pixel format", "Pixel Format Error", MB_OK);
        SysShutdown();
    }

    // Set the pixel format for the provided window DC
    if (!SetPixelFormat(hdc, indexPixelFormat, &pfd))
    {
        MessageBox(hwnd, "Failed to Set Pixel Format", "Pixel Format Error", MB_OK);
        SysShutdown();
    }
}

void Resize(int width, int height)
{
    if (height <= 0)
        height = 1;

    glViewport(0, 0, width, height);

    glMatrixMode(GL_PROJECTION);

    glLoadIdentity();

    glu_perspective(45.0f, (float)width / (float)height, 0.2f, 255.0f);

    glMatrixMode(GL_MODELVIEW);

    glLoadIdentity();
}

// InitOpenGL: initializes OpenGL; resize projection and other setup

void InitOpenGL(void)
{
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClearDepth(1.0f);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);

    glShadeModel(GL_SMOOTH);

    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_FASTEST);
}

//*=====================
//  The event handler
//*=====================

LRESULT CALLBACK WinProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
    switch (msg)
    {
    case WM_CREATE:
    {
        if ((hdc = GetDC(hwnd)) == NULL)				// get device context
        {
            MessageBox(hwnd, "Failed to Get the Window Device Context", "Device Context Error", MB_OK);
            SysShutdown();
            break;
        }

        SetGLFormat();					// select pixel format, needed before wglCreateContext call

        if ((hglrc = wglCreateContext(hdc)) == NULL)	// create the rendering context
        {
            MessageBox(hwnd, "Failed to Create the OpenGL Rendering Context", "OpenGL Rendering Context Error", MB_OK);
            SysShutdown();
            break;
        }

        if ((wglMakeCurrent(hdc, hglrc)) == false)		// make hglrc current rc
        {
            MessageBox(hwnd, "Failed to Make OpenGL Rendering Context Current", "OpenGL Rendering Context Error", MB_OK);
            SysShutdown();
        }

        Resize(SCRWIDTH, SCRHEIGHT);

        InitOpenGL();					// initialize OpenGL before showing the window

        ShowWindow(hwnd, SW_SHOW);		// everything went OK, show the window
        UpdateWindow(hwnd);

        break;
    }

    case WM_DESTROY:
    {
        SysShutdown();					// release memory and shutdown
        break;
    }

    case WM_SIZE:
    {
        // resize the viewport after the window had been resized
        Resize(LOWORD(lparam), HIWORD(lparam));

        screenw = LOWORD(lparam);
        screenh = HIWORD(lparam);

        break;
    }
    }

    return DefWindowProc(hwnd, msg, wparam, lparam);
}

//*=====================
//  WinMain
//*=====================

int WINAPI WinMain(HINSTANCE hinstance, HINSTANCE hprevinstance, LPSTR lpcmdline, int nshowcmd)
{
    MSG msg;
    WNDCLASSEX ex;

    ex.cbSize = sizeof(WNDCLASSEX);
    ex.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
    ex.lpfnWndProc = WinProc;
    ex.cbClsExtra = 0;
    ex.cbWndExtra = 0;
    ex.hInstance = hinstance;
    ex.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    ex.hCursor = LoadCursor(NULL, IDC_ARROW);
    ex.hbrBackground = NULL;
    ex.lpszMenuName = NULL;
    ex.lpszClassName = WNDCLASSNAME;
    ex.hIconSm = NULL;

    if (!RegisterClassEx(&ex))
    {
        MessageBox(NULL, "Failed to register the window class", "Window Reg Error", MB_OK);
        return 1;
    }

    // Create the window
    CreateWnd(hinstance, screenw, screenh, SCRDEPTH, WINDOWED);

    // The message loop
    while (!quit)
    {
        if (PeekMessage(&msg, NULL, NULL, NULL, PM_REMOVE))
        {
            if (msg.message == WM_QUIT)
                quit = true;

            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }

        RenderFrame();

        SwapBuffers(hdc);

        if (GetAsyncKeyState(VK_ESCAPE))
            SysShutdown();
    }

    return msg.lParam;
}
