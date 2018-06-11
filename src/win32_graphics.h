#pragma once


#define _WIN32_WINNT_WIN10 0x0A00
#include <windows.h>		// Standard windows headers
#include <tchar.h>

#include <commctrl.h>		// Common controls dialogs unit
#include <gl\gl.h>			// Header File For The OpenGL32 Library
#include <gl\glu.h>			// Header File For The GLu32 Library

#pragma comment(lib,"comctl32.lib") 
#pragma comment(lib,"OpenGl32.lib")
#pragma comment(lib,"GLU32.lib")


#define IDC_BMPLOAD 101									// App menu to load bitmap
#define IDC_EXIT 105									// App menu command to exit application
#define IDC_TIMERSTART 201								// App menu to start timer
#define IDC_TIMERSTOP 202								// App menu to stop timer
#define IDC_MDICHILD 301								// Create a new MDI child
#define IDC_CASCADE 26									// Cascade MDI children
#define IDC_TILE	27									// Tile MDI children

#define ID_MDI_CLIENT 1									// MDI client id
#define ID_MDI_FIRSTCHILD 50000							// MDI child first id


//LRESULT CALLBACK OpenGLMDIChildHandler(HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam);


typedef struct OpenGLData
{
    HGLRC Rc;
} GLDATABASE;

static HGLRC InitGL(HWND Wnd) {
    HGLRC ourOpenGLRC = 0;											// Preset render context to zero

                                                                    //  We need to make sure the window create in a suitable DC format
    PIXELFORMATDESCRIPTOR pfd = {
        sizeof(PIXELFORMATDESCRIPTOR),
        1,
        PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER, //Flags
        PFD_TYPE_RGBA,												// The kind of framebuffer. RGBA or palette.
        32,															// Colordepth of the framebuffer.
        0, 0, 0, 0, 0, 0,
        0,
        0,
        0,
        0, 0, 0, 0,
        24,															// Number of bits for the depthbuffer
        8,															// Number of bits for the stencilbuffer
        0,															// Number of Aux buffers in the framebuffer.
        PFD_MAIN_PLANE,
        0,
        0, 0, 0
    };

    HDC ourWindowHandleToDeviceContext = GetDC(Wnd);				// Get a DC for our window
    int letWindowsChooseThisPixelFormat = ChoosePixelFormat(ourWindowHandleToDeviceContext, &pfd); // Let windows select an appropriate pixel format
    if (SetPixelFormat(ourWindowHandleToDeviceContext, letWindowsChooseThisPixelFormat, &pfd)) { // Try to set that pixel format
        ourOpenGLRC = wglCreateContext(ourWindowHandleToDeviceContext);
        if (ourOpenGLRC != 0) {
            wglMakeCurrent(ourWindowHandleToDeviceContext, ourOpenGLRC); // Make our render context current
            glEnable(GL_TEXTURE_2D);								// Enable Texture Mapping
            glShadeModel(GL_SMOOTH);								// Enable Smooth Shading
            glClearColor(0.0f, 0.0f, 0.0f, 0.0f);					// Black Background
            glClearDepth(1.0f);										// Depth Buffer Setup
            glEnable(GL_DEPTH_TEST);								// Enables Depth Testing
            glDepthFunc(GL_LEQUAL);									// The Type Of Depth Testing To Do
            glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);		// Really Nice Perspective Calculations
        }
    }
    ReleaseDC(Wnd, ourWindowHandleToDeviceContext);					// Release the window device context we are done
    return (ourOpenGLRC);											// Return the render context
}


LRESULT CALLBACK OpenGLMDIChildHandler(HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
    switch (Msg)
    {
    case WM_CREATE:
    {											// WM_CREATE MESSAGE

        GLDATABASE* db = (GLDATABASE*)malloc(sizeof(GLDATABASE)); // Allocate structure
        db->Rc = InitGL(Wnd);								// Initialize OpenGL and get render context

        SetProp(Wnd, DATABASE_PROPERTY, (HANDLE)db);		// Set the database structure to a property on window
        ReSizeGLScene(Wnd);								// Rescale the OpenGL window
    }
    break;
    case WM_PAINT:
    {											// WM_PAINT MESSAGE
        PAINTSTRUCT Ps;
        GLDATABASE* db = (GLDATABASE*)GetProp(Wnd, DATABASE_PROPERTY);// Fetch the data base
        BeginPaint(Wnd, &Ps);								// Begin paint
        DrawGLScene(db, Ps.hdc);							// Darw the OpenGL scene
        SwapBuffers(Ps.hdc);								// Swap buffers
        EndPaint(Wnd, &Ps);									// End paint
        return 0;
    }
    break;
    }
}




// create window
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    TCHAR* AppClassName = _T("Windows Cipher");
    HACCEL hAccel = 0;												// no accelerator table in this example.
    MSG Msg;
    RECT R;
    HWND Wnd;
    WNDCLASSEX WndClass;

    ZeroMemory(&WndClass, sizeof(WNDCLASSEX));						// Clear the class record
    WndClass.cbSize = sizeof(WNDCLASSEX);							// Size of this record
    WndClass.style = CS_HREDRAW | CS_VREDRAW;						// Set class styles
    WndClass.lpfnWndProc = OpenGLMDIChildHandler;					// Handler for this class
    WndClass.cbWndExtra = 0;										// No extra window data
    WndClass.hInstance = GetModuleHandle(NULL);						// This instance
    WndClass.hIcon = LoadIcon(0, IDI_APPLICATION);					// Set icon
    WndClass.hCursor = LoadCursor(0, IDC_ARROW);					// Set cursor
    WndClass.hbrBackground = (HBRUSH)GetStockObject(NULL_BRUSH);	// Set background brush
    WndClass.lpszMenuName = NULL;									// No menu yet
    WndClass.lpszClassName = MDICHILD_CLASSNAME;					// Set class name
    RegisterClassEx(&WndClass);										// Register the class

    GetClientRect(GetDesktopWindow(), &R);							// Get desktop area					
    Wnd = CreateWindowEx(
        0,
        AppClassName,
        _T("OpenGL MDI Example"),
        WS_VISIBLE | WS_OVERLAPPEDWINDOW,
        500,
        100,
        NULL,
        NULL,
        0, 0, GetModuleHandle(NULL),
        NULL);

    while (true)
    {
        while (PeekMessage(&Msg, PM_REMOVE) == TRUE)
        {
            if (Msg.message == WM_QUIT)
                break;

            TranslateMessage(&Msg);
            DispatchMessage(&Msg);
        }

        // Do your stuff here
    }


    while (GetMessage(&Msg, 0, 0, 0)) {								// Get messages
        if (!TranslateMDISysAccel(AppMDIClient, &Msg) &&			// MDI specific keyboard msgs
            !TranslateAccelerator(Wnd, hAccel, &Msg))				// All other keyboard msgs
        {
            TranslateMessage(&Msg);									// Translate each message
            DispatchMessage(&Msg);									// Dispatch each message
        }
    };
    return (0);
}
// create opengl context

// handle gameloop

// handle inputs