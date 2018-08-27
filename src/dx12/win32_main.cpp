//
//
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
//
//
