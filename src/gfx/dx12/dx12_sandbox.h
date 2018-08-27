
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <shellapi.h> // For CommandLineToArgvW

#include <Shlwapi.h>

#include "dx12/win32_application.h"
#include "dx12/win32_tutorial.h"

#include "opengl/sandbox.h"
#include "dx12/win32_functional.h"
#include <dxgidebug.h>


namespace rain
{
	void StartDirectX12Functional(HINSTANCE hInstance);
	void StartDirectX12(HINSTANCE hInstance);
	void ReportLiveObjects();
}
