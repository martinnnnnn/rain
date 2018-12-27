
#include "engine/win32/helper_win32.h"


int WINAPI WinMain(HINSTANCE hinstance, HINSTANCE hprevinstance, LPSTR lpcmdline, int nshowcmd)
{

	//if (RAIN_APPLICATION.init(hinstance, "") != 0)
	//{
	//	return -1;
	//}

	MSG msg;
	//bool quit = false;
	//while (!quit)
	//{
	//	if (PeekMessage(&msg, NULL, NULL, NULL, PM_REMOVE))
	//	{
	//		if (msg.message == WM_QUIT)
	//			quit = true;

	//		TranslateMessage(&msg);
	//		DispatchMessage(&msg);
	//	}

	//	RAIN_APPLICATION.update();

	//	if (GetAsyncKeyState(VK_ESCAPE))
	//	{
	//		RAIN_WINDOW->shutdown();
	//	}
	//}

	return msg.lParam;
}

