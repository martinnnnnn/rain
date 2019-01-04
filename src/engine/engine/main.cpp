//
//
//#include "engine/win32/win32_helpers.h"
//#include "engine/core/context.h"
//
//
//namespace rain::engine
//{
//    void init_application (Application* _application, HINSTANCE _hinstance)
//    {
//
//    }
//}
//
//int WINAPI WinMain(HINSTANCE hinstance, HINSTANCE hprevinstance, LPSTR lpcmdline, int nshowcmd)
//{
//    rain::engine::Application application;
//    rain::engine::init_application(&application, hinstance);
//
//    MSG msg;
//    bool quit = false;
//    while (!quit)
//    {
//        if (PeekMessage(&msg, NULL, NULL, NULL, PM_REMOVE))
//        {
//            if (msg.message == WM_QUIT)
//                quit = true;
//
//            TranslateMessage(&msg);
//            DispatchMessage(&msg);
//        }
//
//        RAIN_APPLICATION.update();
//
//        if (GetAsyncKeyState(VK_ESCAPE))
//        {
//            RAIN_WINDOW->shutdown();
//        }
//    }
//
//    return msg.lParam;
//}
//
