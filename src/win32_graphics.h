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


LRESULT CALLBACK OpenGLMDIChildHandler(HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam);