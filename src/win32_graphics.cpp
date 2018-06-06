/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++}
{					OPENGL MDI APP DEMO VERSION 1.0					        }
{       Writen by Leon de Boer, Perth, Western Australia, 2016.				}
{	  	contact: ldeboer@gateway.net.au										}
{																			}
{       Copyright released on Code Project Open License (CPOL) and use      }
{       and/or abuse is freely given :-)									}
{																			}
{        THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND		}
{++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

/*==========================================================================
BACKGROUND TO CODE

This code is is C syntax rather than C++ or C# code and contains no objects
or sophisticated elements. That is not because those techniques don't have
merit or a place but simply because this code is targetted as a learning
tool to the widest audience. Anyone proficient in C++ or C# could easily
convert this code to those formats.

==========================================================================*/
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#define _WIN32_WINNT_WIN10 0x0A00
#include <windows.h>		// Standard windows headers
#include <tchar.h>			// Unicode support	.. we will use TCHAR rather than char	
#include <commctrl.h>		// Common controls dialogs unit
#include <gl\gl.h>			// Header File For The OpenGL32 Library
#include <gl\glu.h>			// Header File For The GLu32 Library

// This is the lazy adding libraries via #pragma rather than in linker includes in visual studio
// If you are not on visual studio you will need to comment out the #pragma statements and
// add the libraries to the includes in your compiler linker 
#pragma comment(lib,"comctl32.lib") 
#pragma comment(lib,"OpenGl32.lib")
#pragma comment(lib,"GLU32.lib")

/***************************************************************************
APP SPECIFIC INTERNAL CONSTANTS
***************************************************************************/

/*--------------------------------------------------------------------------}
;{                   MAIN MENU COMMAND VALUE CONSTANTS			            }
;{-------------------------------------------------------------------------*/
#define IDC_BMPLOAD 101									// App menu to load bitmap
#define IDC_EXIT 105									// App menu command to exit application
#define IDC_TIMERSTART 201								// App menu to start timer
#define IDC_TIMERSTOP 202								// App menu to stop timer
#define IDC_MDICHILD 301								// Create a new MDI child
#define IDC_CASCADE 26									// Cascade MDI children
#define IDC_TILE	27									// Tile MDI children

/*--------------------------------------------------------------------------}
;{                         MDI CLIENT CONSTANTS					            }
;{-------------------------------------------------------------------------*/
#define ID_MDI_CLIENT 1									// MDI client id
#define ID_MDI_FIRSTCHILD 50000							// MDI child first id

/*--------------------------------------------------------------------------}
;{                      APPLICATION STRING CONSTANTS			            }
;{-------------------------------------------------------------------------*/
static const TCHAR* MDICHILD_CLASSNAME = _T("OpenGl_MDI_Child");
static const TCHAR* DATABASE_PROPERTY = _T("OurDataStructure");

/*--------------------------------------------------------------------------}
;{                         APPLICATION GLOBAL VARIABLES			            }
;{-------------------------------------------------------------------------*/
static HWND AppMDIClient = 0;							// Application MDI client handle
static char* OpenGLVersion = 0;							// OpenGL version string is always ansi never unicode

                                                        /*--------------------------------------------------------------------------}
                                                        ;{                         MDI CLIENT DATA STRUCT				            }
                                                        ;{-------------------------------------------------------------------------*/

                                                        /*---------------------------------------------------------------------------
                                                        MDICHILD DATA RECORD DEFINITION
                                                        ---------------------------------------------------------------------------*/
typedef struct OpenGLData {
    HGLRC Rc;											// Our render context
    GLuint glTexture;									// Our texture to draw
    GLfloat	xrot;										// X Rotation
    GLfloat	yrot;										// Y Rotation
} GLDATABASE;

/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
COMMON DIALOG CALL ROUTINES
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

/*--------------------------------------------------------------------------
This uses file open common control from windows to select a file.
--------------------------------------------------------------------------*/
int OpenFileDialog(TCHAR* Name, unsigned short NameBufSize, TCHAR* String, TCHAR* Ext, TCHAR* Title, HWND Wnd) {
    int i;
    TCHAR FileName[256], DefExt[256], Filter[400];
    OPENFILENAME OpenFN;

    InitCommonControls();											// Initialize common dialogs
    Name[0] = 0;													// Preset name clear
    _tcscpy_s(&FileName[0], _countof(FileName), _T("*."));			// Tranfer "*."
    _tcscat_s(&FileName[0], _countof(FileName), Ext);				// Create "*.xxx" extension
    _tcscpy_s(Filter, _countof(Filter), String);					// Tranfer string
    i = _tcslen(Filter);											// Fetch that string length in TCHAR
    _tcscpy_s(&Filter[i + 1], _countof(Filter) - i - 1, &FileName[0]);	// Transfer "*.ext"
    i += (_tcslen(&FileName[0]) + 1);								// Advance to beyond end
    _tcscpy_s(&Filter[i + 1], _countof(Filter) - i - 1, _T("\0"));	// Must end with two 0 entries
    _tcscpy_s(&Filter[i + 2], _countof(Filter) - i - 2, _T("\0"));	// Must end with two 0 entries
    _tcscpy_s(DefExt, _countof(DefExt), Ext);						// Default ext name
    memset((void*)&OpenFN, 0, sizeof(OpenFN));					    // Zero unused fields
    OpenFN.lStructSize = sizeof(OPENFILENAME);					    // Size of structure
    OpenFN.hInstance = GetModuleHandle(NULL);						// Pointer to instance
    OpenFN.hwndOwner = Wnd;											// Owner window
    OpenFN.lpstrFilter = &Filter[0];								// Filter
    OpenFN.nFilterIndex = 1;										// 1st Filter String
    OpenFN.lpstrFile = &FileName[0];								// Return result
    OpenFN.nMaxFile = _countof(FileName);							// Max name length
    OpenFN.lpstrDefExt = Ext;										// Default extension
    OpenFN.lpstrFileTitle = &FileName[0];							// Default file title
    OpenFN.nMaxFileTitle = _countof(FileName);						// Max size of file title
    OpenFN.lpstrTitle = Title;	                     			    // Window title
    OpenFN.lpfnHook = NULL;											// No hook handler
    OpenFN.Flags = OFN_FILEMUSTEXIST | OFN_LONGNAMES;               // Set flag masks
    if (GetOpenFileName(&OpenFN) != 0) {
        _tcscpy_s(Name, NameBufSize, FileName);						// Return the name
        return OpenFN.nFilterIndex;									// Return filter type
    }
    else return 0;												// Dialog cancelled
}

/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
OPENGL ROUTINES
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

/*-[ InitGL ]---------------------------------------------------------------

Initializes the OpenGL system for the provided window handle, this is a one
time call made for the window and the function returns the created Render
context for the window. The responsibility to delete the render context when
completed is placed on the caller. In this demo initialization will be called
from WM_CREATE from each MDICLIENT window. Failure of initialization will
return a render context handle equal to zero.

16Apr16 LdB
--------------------------------------------------------------------------*/
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


/*-[ ReSizeGLScene ]--------------------------------------------------------

Rescales the OpenGL system for a given size of screen, called at anytime
the Application resizes the window . It will call once after InitGL as
in this demo it is called from WM_WINDOWPOSCHANGING/WM_WINDOWPOSCHANGED
from the MDICLIENT windows.

15Apr16 LdB
--------------------------------------------------------------------------*/
static void ReSizeGLScene(HWND Wnd) {
    GLDATABASE* db = (GLDATABASE*)GetProp(Wnd, DATABASE_PROPERTY); // Fetch the data base
    if (db == 0) return;											// Cant resize .. no render context
    HDC Dc = GetWindowDC(Wnd);										// Get the window DC
    RECT r;
    GetWindowRect(Wnd, &r);											// Fetch the window size
    int Width = r.right - r.left;									// Window width
    int Height = r.bottom - r.top;									// Window height
    if (Height == 0) Height = 1;									// Stop divid by zero
    wglMakeCurrent(Dc, db->Rc);										// Make our render context current
    glViewport(0, 0, Width, Height);								// Reset The Current Viewport
    glMatrixMode(GL_PROJECTION);									// Select The Projection Matrix
    glLoadIdentity();												// Reset The Projection Matrix
                                                                    // Calculate The Aspect Ratio Of The Window
    gluPerspective(45.0f, (GLfloat)Width / (GLfloat)Height, 0.1f, 100.0f);

    glMatrixMode(GL_MODELVIEW);										// Select The Modelview Matrix
    glLoadIdentity();												// Reset The Modelview Matrix
    ReleaseDC(Wnd, Dc);												// Release the window DC
}


/*-[ DrawGLScene ]----------------------------------------------------------

This is where all the OpenGL drawing is done for each frame. In this demo
it will be called from WM_PAINT messages to the window

15Apr16 LdB
--------------------------------------------------------------------------*/

void DrawGLScene(GLDATABASE* db, HDC Dc) {
    if ((db == 0) || (db->glTexture == 0)) return;					// Cant draw .. no render context
    wglMakeCurrent(Dc, db->Rc);										// Make our render context current

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);				// Clear The Screen And The Depth Buffer
    glLoadIdentity();												// Reset The View
    glTranslatef(0.0f, 0.0f, -5.0f);

    glRotatef(db->xrot, 1.0f, 0.0f, 0.0f);
    glRotatef(db->yrot, 0.0f, 1.0f, 0.0f);

    glBindTexture(GL_TEXTURE_2D, db->glTexture);

    glBegin(GL_QUADS);
    // Front Face
    glNormal3f(0.0f, 0.0f, 1.0f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f, 1.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(1.0f, -1.0f, 1.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(1.0f, 1.0f, 1.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f, 1.0f, 1.0f);
    // Back Face
    glNormal3f(0.0f, 0.0f, -1.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f, -1.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f, 1.0f, -1.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(1.0f, 1.0f, -1.0f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(1.0f, -1.0f, -1.0f);
    // Top Face
    glNormal3f(0.0f, 1.0f, 0.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f, 1.0f, -1.0f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, 1.0f, 1.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(1.0f, 1.0f, 1.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(1.0f, 1.0f, -1.0f);
    // Bottom Face
    glNormal3f(0.0f, -1.0f, 0.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f, -1.0f, -1.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(1.0f, -1.0f, -1.0f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(1.0f, -1.0f, 1.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f, 1.0f);
    // Right face
    glNormal3f(1.0f, 0.0f, 0.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(1.0f, -1.0f, -1.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(1.0f, 1.0f, -1.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(1.0f, 1.0f, 1.0f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(1.0f, -1.0f, 1.0f);
    // Left Face
    glNormal3f(-1.0f, 0.0f, 0.0f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f, -1.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f, 1.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f, 1.0f, 1.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f, 1.0f, -1.0f);
    glEnd();
}


/*-[ BMP2GLTexture ]--------------------------------------------------------

This converts a bitmap file from the filename path and converts it to an
OpenGL texture. If the filename is invalid of the file not a bitmap the
routine will return a GLuint of zero.

15Apr16 LdB
--------------------------------------------------------------------------*/
GLuint BMP2GLTexture(TCHAR* fileName, HWND Wnd, GLDATABASE* db) {
    HBITMAP hBMP;                                                   // Handle Of The Bitmap
    BITMAP  BMP;                                                    // Bitmap Structure

    HDC Dc = GetWindowDC(Wnd);										// Fetch the window DC
    if ((db != 0) && (db->Rc != 0)) wglMakeCurrent(Dc, db->Rc);		// Make our render context current

    hBMP = (HBITMAP)LoadImage(GetModuleHandle(NULL), fileName,
        IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION | LR_LOADFROMFILE);	// Load the bitmap from file
    if (!hBMP) return (0);											// If bitmap does not exist return false
    GetObject(hBMP, sizeof(BMP), &BMP);								// Get The bitmap details
    int P2Width = (BMP.bmWidth) >> 2;								// Divid width by 4
    if ((P2Width << 2) < (BMP.bmWidth)) P2Width++;					// Inc by 1 if width x 4 is less than original
    P2Width = P2Width << 2;											// Power of two width
    long imageSize = (long)P2Width * (long)BMP.bmHeight * sizeof(RGBQUAD);
    BYTE* lpPixels = (BYTE*)malloc(imageSize);						// Create the pixel buffer					

                                                                    // Create and fill BITMAPINFO structure to pass to GetDIBits
    BITMAPINFO bmi;
    bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    bmi.bmiHeader.biWidth = BMP.bmWidth;
    bmi.bmiHeader.biHeight = BMP.bmHeight;
    bmi.bmiHeader.biPlanes = 1;
    bmi.bmiHeader.biBitCount = 24;
    bmi.bmiHeader.biCompression = 0;
    bmi.bmiHeader.biSizeImage = imageSize;
    bmi.bmiHeader.biXPelsPerMeter = 0;
    bmi.bmiHeader.biYPelsPerMeter = 0;
    bmi.bmiHeader.biClrUsed = 0;
    bmi.bmiHeader.biClrImportant = 0;

    // Put DIBBits into memory buffer
    GetDIBits(Dc, hBMP, 0, BMP.bmHeight, lpPixels, &bmi, DIB_RGB_COLORS);

    glPixelStorei(GL_UNPACK_ALIGNMENT, 4);							// Pixel Storage Mode (Word Alignment / 4 Bytes)
    GLuint texture;
    glGenTextures(1, &texture);										// Create a GL texture

                                                                    // Create Nearest Filtered Texture
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, 3, BMP.bmWidth, BMP.bmHeight, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, lpPixels);

    free(lpPixels);													// Free allocated pixel memory
    ReleaseDC(Wnd, Dc);												// Release the window DC
    DeleteObject(hBMP);												// Delete The Object
    return (texture);												// Return the texture
}

/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
MDI CHILD LEVEL ROUTINES
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

/*--------------------------------------------------------------------------
OpenGL MDICHILD handler
--------------------------------------------------------------------------*/
static LRESULT CALLBACK OpenGLMDIChildHandler(HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam) {
    switch (Msg) {
    case WM_CREATE: {											// WM_CREATE MESSAGE
                                                                // Drag and drop functionality added
        DragAcceptFiles(Wnd, TRUE);

        GLDATABASE* db = (GLDATABASE*)malloc(sizeof(GLDATABASE)); // Allocate structure
        db->Rc = InitGL(Wnd);								// Initialize OpenGL and get render context
        db->glTexture = 0;									// Zero the texture
        db->xrot = 0.0f;									// Zero x rotation
        db->yrot = 0.0f;									// Zero y rotation
        SetProp(Wnd, DATABASE_PROPERTY, (HANDLE)db);		// Set the database structure to a property on window
        ReSizeGLScene(Wnd);								// Rescale the OpenGL window
    }
                    break;
    case WM_DESTROY: {											// WM_DESTROY MESSAGE
        GLDATABASE* db = (GLDATABASE*)GetProp(Wnd, DATABASE_PROPERTY); // Fetch the data base
        if (db != 0) {
            if (db->Rc != 0) wglDeleteContext(db->Rc);		// If valid delete the rendering context
            if (db->glTexture != 0) {
                HDC Dc = GetWindowDC(Wnd);					// Get a window context
                wglMakeCurrent(Dc, db->Rc);					// Make sure our render context current
                glDeleteTextures(1, &db->glTexture);		// If valid delete the texture
                ReleaseDC(Wnd, Dc);							// Release the window context
            }
            free(db);										// Release the data structure memory
        }
    }
                     break;
    case WM_PAINT: {											// WM_PAINT MESSAGE
        PAINTSTRUCT Ps;
        GLDATABASE* db = (GLDATABASE*)GetProp(Wnd, DATABASE_PROPERTY);// Fetch the data base
        BeginPaint(Wnd, &Ps);								// Begin paint
        DrawGLScene(db, Ps.hdc);							// Darw the OpenGL scene
        SwapBuffers(Ps.hdc);								// Swap buffers
        EndPaint(Wnd, &Ps);									// End paint
        return 0;
    }
                   break;
    case WM_TIMER: {											// WM_TIMER MESSAGE
        GLDATABASE* db = (GLDATABASE*)GetProp(Wnd, DATABASE_PROPERTY);// Fetch the data base
        db->xrot += 1.0f;									// Inc x rotation
        db->yrot += 1.0f;									// Inc y rotation
        InvalidateRect(Wnd, 0, TRUE);						// We need a redraw now so invalidate us			
    }
                   break;
    case WM_WINDOWPOSCHANGED:									// WM_WINDOWPOSCHANGED
                                                                // Check if window size has changed .. window move doesnt change aspect ratio
        if ((lParam == 0) || ((((PWINDOWPOS)lParam)->flags & SWP_NOSIZE) == 0)) {
            ReSizeGLScene(Wnd);									// Rescale the GL window							
            InvalidateRect(Wnd, 0, TRUE);						// We need a redraw now so invalidate us
        }
        break;
    case WM_ERASEBKGND:											// WM_ERASEBKGND MESSAGE
        return (FALSE);
    case WM_DROPFILES: {										// WM_DROPFILES ... Added for drag and drop support
        TCHAR szFilename[MAX_PATH];
        DragQueryFile((HDROP)wParam, 0, &szFilename[0], MAX_PATH);
        DragFinish((HDROP)wParam);
        if (_tcsstr(szFilename, _T(".bmp")) || _tcsstr(szFilename, _T(".BMP"))) {
            GLDATABASE* db = (GLDATABASE*)GetProp(Wnd, DATABASE_PROPERTY);
            if (db != 0) {
                // Now check if texture exists and if so delete it
                if (db->glTexture) {
                    HDC Dc = GetWindowDC(Wnd);					// Get a window context
                    wglMakeCurrent(Dc, db->Rc);					// Make sure our render context current
                    glDeleteTextures(1, &db->glTexture);
                    ReleaseDC(Wnd, Dc);							// Release the window context
                    db->glTexture = 0;
                }
                // Create new texture
                db->glTexture = BMP2GLTexture(&szFilename[0], Wnd, db);
                InvalidateRect(Wnd, 0, TRUE);					// Force redraw of window
            }
        }
        return 0;
    }
    }
    return DefMDIChildProc(Wnd, Msg, wParam, lParam);				// Pass unprocessed message to DefMDIChildProc
}


/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
APPLICATION LEVEL ROUTINES
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

/*--------------------------------------------------------------------------
Application handler
--------------------------------------------------------------------------*/
static LRESULT CALLBACK AppHandler(HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam) {
    switch (Msg) {
    case WM_CREATE: {											// WM_CREATE MESSAGE
                                                                // Drag and drop functionality added
        DragAcceptFiles(Wnd, TRUE);

        // We are going to manually build a menu for the application
        // You could do this by resource file but this is another way

        HMENU SubMenu, Menu;
        Menu = CreateMenu();									// Create main menu item
                                                                // Create a submenu and populate it
        SubMenu = CreatePopupMenu();							// Create a submenu popup
        AppendMenu(SubMenu, MF_STRING, IDC_BMPLOAD, _T("&Load Bitmap"));
        AppendMenu(SubMenu, MF_SEPARATOR, 0, NULL);
        AppendMenu(SubMenu, MF_STRING, IDC_EXIT, _T("E&xit"));
        // Append the above popup submenu into our menu
        AppendMenu(Menu, MF_POPUP, (UINT_PTR)SubMenu, _T("&File"));
        // Create another submenu and populate it
        SubMenu = CreatePopupMenu();
        AppendMenu(SubMenu, MF_STRING, IDC_TIMERSTART, _T("&Start Timer"));
        AppendMenu(SubMenu, MF_STRING, IDC_TIMERSTOP, _T("Stop &Timer"));
        // Append the above popup submenu into our menu
        AppendMenu(Menu, MF_POPUP, (UINT_PTR)SubMenu, _T("&Timers"));
        // Create another submenu and populate it
        SubMenu = CreatePopupMenu();
        AppendMenu(SubMenu, MF_STRING, IDC_MDICHILD, _T("&New OpenGL window"));
        AppendMenu(SubMenu, MF_SEPARATOR, 0, NULL);
        AppendMenu(SubMenu, MF_STRING, IDC_TILE, _T("&Tile"));
        AppendMenu(SubMenu, MF_STRING, IDC_CASCADE, _T("&Cascade"));
        // Append the above popup submenu into our menu
        AppendMenu(Menu, MF_POPUP, (UINT_PTR)SubMenu, _T("&Windows"));
        // Set the main menu item as our windows menu
        SetMenu(Wnd, Menu);

        // We now need an MDICLIENT for our MDI clients to work in
        // This is like an invisible plane which is inserted into
        // the normal windows working area that the MDI clients use

        CLIENTCREATESTRUCT ccs;
        ccs.hWindowMenu = GetSubMenu(GetMenu(Wnd), 0);			// Menu handle for menu item in our case bottom of Window menu 
        ccs.idFirstChild = ID_MDI_FIRSTCHILD;					// Id of the first mdi child window
        AppMDIClient = CreateWindowEx(0, _T("mdiclient"), NULL,
            WS_VISIBLE | WS_CHILD | WS_CLIPCHILDREN |
            WS_VSCROLL | WS_HSCROLL,
            0, 0, 0, 0, Wnd,
            (HMENU)ID_MDI_CLIENT, GetModuleHandle(0),
            (void*)(&ccs));									// Create MDI client child window
        return (0);												// Return message handled
    }
    case WM_DESTROY:											// WM_DESTROY MESSAGE
        PostQuitMessage(0);										// Post quit message
        return (0);												// Return message handled
    case WM_COMMAND: {
        switch LOWORD(wParam) {
        case IDC_BMPLOAD: {                                 // Menu item: File-->Load Bitmap
            TCHAR FileName[256];
            int i = OpenFileDialog(&FileName[0], _countof(FileName),
                _T("BMP file:"), _T("BMP"), _T("BMP FILE TO LOAD"), Wnd);
            if (i != 0) {
                // Fetch the active MDI child
                HWND Child = (HWND)SendMessage(AppMDIClient, WM_MDIGETACTIVE, 0, 0);
                // No child selected so make one
                if (Child == 0) {
                    Child = CreateWindowEx(WS_EX_MDICHILD,
                        MDICHILD_CLASSNAME, _T("OpenGL MDI Client"),
                        WS_CHILD | WS_VISIBLE | WS_OVERLAPPEDWINDOW,
                        CW_USEDEFAULT, CW_USEDEFAULT,
                        CW_USEDEFAULT, CW_USEDEFAULT,
                        AppMDIClient, NULL, GetModuleHandle(0),
                        NULL);							// Create the MDI Child
                }
                if (Child != 0) {
                    // Fetch that childs data base
                    GLDATABASE* db = (GLDATABASE*)GetProp(Child, DATABASE_PROPERTY);
                    if (db != 0) {
                        // Now check if texture exists and if so delete it
                        if (db->glTexture) {
                            HDC Dc = GetWindowDC(Child); // Get a window context
                            wglMakeCurrent(Dc, db->Rc);	 // Make sure our render context current
                            glDeleteTextures(1, &db->glTexture);
                            ReleaseDC(Child, Dc);		 // Release the window context
                            db->glTexture = 0;
                        }
                        // Create new texture
                        db->glTexture = BMP2GLTexture(&FileName[0], Child, db);
                        InvalidateRect(Child, 0, TRUE);	// Force redraw of window
                    }
                }
            }
        }
                          break;
        case IDC_MDICHILD: {								// Menu item: Window-->New OpenGL Window
            CreateWindowEx(WS_EX_MDICHILD,
                MDICHILD_CLASSNAME, _T("OpenGL MDI Client"),
                WS_CHILD | WS_VISIBLE | WS_OVERLAPPEDWINDOW,
                CW_USEDEFAULT, CW_USEDEFAULT,
                CW_USEDEFAULT, CW_USEDEFAULT,
                AppMDIClient, NULL, GetModuleHandle(0),
                NULL);										// Create the MDI Child
        }
                           break;
        case IDC_TIMERSTART: {								// Menu item: Timer-->Start timer
                                                            // Fetch the active MDI child
            HWND Child = (HWND)SendMessage(AppMDIClient, WM_MDIGETACTIVE, 0, 0);
            if (Child != 0)								// If we have active child set a timer in it
                SetTimer(Child,							 // handle to child window 
                    1,									// timer identifier 
                    100,								// 100 ms interval 
                    0);									// timer callback null
        }
                             break;
        case IDC_TIMERSTOP: {								// Menu item: Timer-->Stop timer
                                                            // Fetch the active MDI child
            HWND Child = (HWND)SendMessage(AppMDIClient, WM_MDIGETACTIVE, 0, 0);
            if (Child != 0) KillTimer(Child, 1);		// If child valid kill the timer
        }
                            break;
        case IDC_EXIT:										// Menu item: File-->Exit
            DestroyWindow(Wnd);
            break;
        case IDC_TILE:										// Menu item: Window-->Tile
            SendMessage(AppMDIClient, WM_MDITILE, 0, 0);
            break;
        case IDC_CASCADE:									// Menu item: Window-->cascade
            SendMessage(AppMDIClient, WM_MDICASCADE, 0, 0);
            break;
        };// End of switch wParam case

          //must call DefFrameProc to ensure that min/max/close default behaviour
          //of mdi child windows can occur.
        return DefFrameProc(Wnd, AppMDIClient, WM_COMMAND, wParam, lParam);
    }
    case WM_DROPFILES: {										// WM_DROPFILES ... Added for drag and drop support
        TCHAR szFilename[MAX_PATH];
        DragQueryFile((HDROP)wParam, 0, &szFilename[0], MAX_PATH);
        DragFinish((HDROP)wParam);
        if (_tcsstr(szFilename, _T(".bmp")) || _tcsstr(szFilename, _T(".BMP"))) {
            HWND Child = CreateWindowEx(WS_EX_MDICHILD,
                MDICHILD_CLASSNAME, _T("OpenGL MDI Client"),
                WS_CHILD | WS_VISIBLE | WS_OVERLAPPEDWINDOW,
                CW_USEDEFAULT, CW_USEDEFAULT,
                CW_USEDEFAULT, CW_USEDEFAULT,
                AppMDIClient, NULL, GetModuleHandle(0),
                NULL);										// Create the MDI Child
            GLDATABASE* db = (GLDATABASE*)GetProp(Child, DATABASE_PROPERTY);
            if (db != 0) {
                // Now check if texture exists and if so delete it
                if (db->glTexture) {
                    HDC Dc = GetWindowDC(Wnd);					// Get a window context
                    wglMakeCurrent(Dc, db->Rc);					// Make sure our render context current
                    glDeleteTextures(1, &db->glTexture);
                    ReleaseDC(Wnd, Dc);							// Release the window context
                    db->glTexture = 0;
                }
                // Create new texture
                db->glTexture = BMP2GLTexture(&szFilename[0], Child, db);
                InvalidateRect(Child, 0, TRUE);				// Force redraw of window
            }
        }
        return 0;
    }
    default:
        return DefFrameProc(Wnd, AppMDIClient, Msg, wParam, lParam);// Default handler
    };// end switch case
    return (0);
};


/*--------------------------------------------------------------------------
Application entry point
--------------------------------------------------------------------------*/

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    TCHAR* AppClassName = _T("Windows Cipher");
    HACCEL hAccel = 0;												// no accelerator table in this example.
    MSG Msg;
    RECT R;
    HWND Wnd;
    WNDCLASSEX WndClass;
    HBRUSH MDIChildBrush;

    // Initialize the common controls dll, specifying the type of control(s) required 
    INITCOMMONCONTROLSEX iccx;
    iccx.dwSize = sizeof(INITCOMMONCONTROLSEX);
    iccx.dwICC = ICC_COOL_CLASSES | ICC_BAR_CLASSES | ICC_DATE_CLASSES;
    InitCommonControlsEx(&iccx);

    MDIChildBrush = CreateSolidBrush(RGB(0, 0x2b, 0x36));			// Create MDI client brush

                                                                    // MDI child class registration
    ZeroMemory(&WndClass, sizeof(WNDCLASSEX));						// Clear the class record
    WndClass.cbSize = sizeof(WNDCLASSEX);							// Size of this record
    WndClass.style = CS_HREDRAW | CS_VREDRAW;						// Set class styles
    WndClass.lpfnWndProc = OpenGLMDIChildHandler;					// Handler for this class
    WndClass.cbClsExtra = 0;										// No extra class data
    WndClass.cbWndExtra = 0;										// No extra window data
    WndClass.hInstance = GetModuleHandle(NULL);						// This instance
    WndClass.hIcon = LoadIcon(0, IDI_APPLICATION);					// Set icon
    WndClass.hCursor = LoadCursor(0, IDC_ARROW);					// Set cursor
    WndClass.hbrBackground = (HBRUSH)GetStockObject(NULL_BRUSH);	// Set background brush
    WndClass.lpszMenuName = NULL;									// No menu yet
    WndClass.lpszClassName = MDICHILD_CLASSNAME;					// Set class name
    RegisterClassEx(&WndClass);										// Register the class

                                                                    // Application class registration
    ZeroMemory(&WndClass, sizeof(WNDCLASSEX));						// Clear the class record
    WndClass.cbSize = sizeof(WNDCLASSEX);							// Size of this record
    WndClass.lpfnWndProc = AppHandler;								// Handler for this class
    WndClass.cbClsExtra = 0;										// No extra class data
    WndClass.cbWndExtra = 0;										// No extra window data
    WndClass.hInstance = GetModuleHandle(NULL);						// This instance
    WndClass.hIcon = LoadIcon(0, IDI_APPLICATION);					// Set icon
    WndClass.hCursor = LoadCursor(0, IDC_ARROW);					// Set cursor
    WndClass.hbrBackground = (HBRUSH)GetStockObject(NULL_BRUSH);	// Set background brush
    WndClass.lpszMenuName = _T("CIPMENU");							// Cipher menu
    WndClass.lpszClassName = AppClassName;							// Set class name
    RegisterClassEx(&WndClass);										// Register the class

    GetClientRect(GetDesktopWindow(), &R);							// Get desktop area					
    Wnd = CreateWindowEx(0, AppClassName, _T("OpenGL MDI Example"),
        WS_VISIBLE | WS_OVERLAPPEDWINDOW, R.left + 50, R.top + 50,
        R.right - R.left - 100, R.bottom - R.top - 100,
        0, 0, GetModuleHandle(NULL),
        NULL);														// Create main window
    while (GetMessage(&Msg, 0, 0, 0)) {								// Get messages
        if (!TranslateMDISysAccel(AppMDIClient, &Msg) &&			// MDI specific keyboard msgs
            !TranslateAccelerator(Wnd, hAccel, &Msg))				// All other keyboard msgs
        {
            TranslateMessage(&Msg);									// Translate each message
            DispatchMessage(&Msg);									// Dispatch each message
        }
    };
    DeleteObject(MDIChildBrush);									// Dispose of MID child brush
    return (0);
}

