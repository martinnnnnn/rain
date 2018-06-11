//#pragma once
//
//
//#define _WIN32_WINNT_WIN10 0x0A00
//#include <windows.h>		// Standard windows headers
//#include <tchar.h>
//
//#include <commctrl.h>		// Common controls dialogs unit
//#include <gl\gl.h>			// Header File For The OpenGL32 Library
//#include <gl\glu.h>			// Header File For The GLu32 Library
//
//#pragma comment(lib,"comctl32.lib") 
//#pragma comment(lib,"OpenGl32.lib")
//#pragma comment(lib,"GLU32.lib")
//
//
//#define IDC_BMPLOAD 101									// App menu to load bitmap
//#define IDC_EXIT 105									// App menu command to exit application
//#define IDC_TIMERSTART 201								// App menu to start timer
//#define IDC_TIMERSTOP 202								// App menu to stop timer
//#define IDC_MDICHILD 301								// Create a new MDI child
//#define IDC_CASCADE 26									// Cascade MDI children
//#define IDC_TILE	27									// Tile MDI children
//
//#define ID_MDI_CLIENT 1									// MDI client id
//#define ID_MDI_FIRSTCHILD 50000							// MDI child first id
//
//
////LRESULT CALLBACK OpenGLMDIChildHandler(HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam);
//
//
//typedef struct OpenGLData
//{
//    HGLRC Rc;
//} GLDATABASE;
//
//static const TCHAR* DATABASE_PROPERTY = _T("OurDataStructure");
//
//static HGLRC InitGL(HWND Wnd)
//{
//    HGLRC ourOpenGLRC = 0;
//
//                                                                    
//    PIXELFORMATDESCRIPTOR pfd =
//    {
//        sizeof(PIXELFORMATDESCRIPTOR),
//        1,
//        PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER, 
//        PFD_TYPE_RGBA,												
//        32,															
//        0, 0, 0, 0, 0, 0,
//        0,
//        0,
//        0,
//        0, 0, 0, 0,
//        24,															
//        8,															
//        0,															
//        PFD_MAIN_PLANE,
//        0,
//        0, 0, 0
//    };
//
//    HDC ourWindowHandleToDeviceContext = GetDC(Wnd);
//    int letWindowsChooseThisPixelFormat = ChoosePixelFormat(ourWindowHandleToDeviceContext, &pfd);
//    if (SetPixelFormat(ourWindowHandleToDeviceContext, letWindowsChooseThisPixelFormat, &pfd))
//    {
//        ourOpenGLRC = wglCreateContext(ourWindowHandleToDeviceContext);
//        if (ourOpenGLRC != 0) {
//            wglMakeCurrent(ourWindowHandleToDeviceContext, ourOpenGLRC);
//            // opengl initialisation
//
//        }
//    }
//    ReleaseDC(Wnd, ourWindowHandleToDeviceContext);
//    return (ourOpenGLRC);
//}
//
//
//LRESULT CALLBACK RainWinProc(HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam)
//{
//    switch (Msg)
//    {
//    case WM_CREATE:
//    {
//        GLDATABASE* db = (GLDATABASE*)malloc(sizeof(GLDATABASE));
//        db->Rc = InitGL(Wnd);
//
//        SetProp(Wnd, DATABASE_PROPERTY, (HANDLE)db);
//        //ReSizeGLScene(Wnd);
//    }
//    break;
//    case WM_PAINT:
//    {											// WM_PAINT MESSAGE
//        PAINTSTRUCT Ps;
//        GLDATABASE* db = (GLDATABASE*)GetProp(Wnd, DATABASE_PROPERTY);
//        BeginPaint(Wnd, &Ps);
//        //DrawGLScene(db, Ps.hdc);
//        SwapBuffers(Ps.hdc);
//        EndPaint(Wnd, &Ps);
//        return 0;
//    }
//    break;
//    }
//}
//
//
//
//
//// create window
//int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
//{
//    TCHAR* AppClassName = _T("Windows Cipher");
//    HACCEL hAccel = 0;												// no accelerator table in this example.
//    MSG Msg;
//    RECT R;
//    HWND Wnd;
//    WNDCLASSEX WndClass;
//
//    ZeroMemory(&WndClass, sizeof(WNDCLASSEX));
//    WndClass.cbSize = sizeof(WNDCLASSEX);
//    WndClass.style = CS_HREDRAW | CS_VREDRAW;
//    WndClass.lpfnWndProc = RainWinProc;
//    WndClass.cbWndExtra = 0;
//    WndClass.hInstance = GetModuleHandle(NULL);
//    WndClass.hIcon = LoadIcon(0, IDI_APPLICATION);
//    WndClass.hCursor = LoadCursor(0, IDC_ARROW);
//    WndClass.hbrBackground = (HBRUSH)GetStockObject(NULL_BRUSH);
//    WndClass.lpszMenuName = NULL;
//    WndClass.lpszClassName = _T("doesntmatter");
//    RegisterClassEx(&WndClass);
//
//    GetClientRect(GetDesktopWindow(), &R);
//    Wnd = CreateWindowEx(
//        0,
//        AppClassName,
//        _T("OpenGL MDI Example"),
//        WS_VISIBLE | WS_OVERLAPPEDWINDOW,
//        500,
//        100,
//        NULL,
//        NULL,
//        0, 0, GetModuleHandle(NULL),
//        NULL);
//
//    while (true)
//    {
//        while (PeekMessage(&Msg, NULL, 0, 0, PM_REMOVE) == TRUE)
//        {
//            if (Msg.message == WM_QUIT)
//                break;
//
//            TranslateMessage(&Msg);
//            DispatchMessage(&Msg);
//        }
//
//        // Do your stuff here
//    }
//    return (0);
//}
//// create opengl context
//
//// handle gameloop
//
//// handle inputs