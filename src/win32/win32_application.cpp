#include "win32_application.h"

#include <windows.h>

#include "win32_window.h"
#include "ogl/ogl_renderer.h"




int Application::init(HINSTANCE _hinstance, const std::string& _config)
{
    hinstance = _hinstance;
    
    /*std::vector<SparseSet*> pools;

    using fam = TypeId<struct Hey>;

    if (fam::get<Transform>() >= pools.size())
    {
        pools.resize(fam::get<Transform>() + 1);
    }

    pools[fam::get<Transform>()] = new Pool<Transform>();

    if (fam::get<Name>() >= pools.size())
    {
        pools.resize(fam::get<Name>() + 1);
    }

    pools[fam::get<Name>()] = new Pool<Name>();*/


    GETWINDOW.initialize(hinstance, 800, 600, 0);

    renderer = new Renderer();
    renderer->initialize();
    GETWINDOW.show();

    return 0;
}

void Application::update()
{
    
}

void Application::render()
{
    renderer->render();
}


int WINAPI WinMain(HINSTANCE hinstance, HINSTANCE hprevinstance, LPSTR lpcmdline, int nshowcmd)
{
    Application* app = new Application();
    app->init(hinstance, "");

	MSG msg;
	bool quit = false;
	while (!quit)
	{
		if (PeekMessage(&msg, NULL, NULL, NULL, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
				quit = true;

			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

        // *** game logic update *** // 
        app->update();
        // *** needs a better game loop too *** // 

		if (GETWINDOW.is_initialized())
		{
			app->render();
			GETWINDOW.present();
		}

        if (GetAsyncKeyState(VK_ESCAPE))
        {
            GETWINDOW.shutdown();
        }
	}

	return msg.lParam;
}
