#include "win32_application.h"

#include <windows.h>

#include "win32_window.h"
#include "ogl/ogl_renderer.h"

#include "ecs/ecs_entity.h"
#include "ecs/ecs_registry.h"

void set_test()
{
    SparseSet set;
    bool empty1 = set.empty();

    set.construct(32);
    set.construct(62);
    set.construct(12);
    set.construct(42);

    bool empty2 = set.empty();

    bool c1 = set.contains(32);
    //bool c2 = set.contains(-1);
    bool c3 = set.contains(123);

    u32 size1 = set.get(62);


    //set.destroy(-1);
    set.destroy(42);
    //set.destroy(123);
    set.destroy(62);

}

int Application::init(HINSTANCE _hinstance, const std::string& _config)
{
    hinstance = _hinstance;

	Registry registry;

    Entity e1 = registry.create();
    Entity e2 = registry.create();
    Entity e3 = registry.create();
    Entity e4 = registry.create();

	registry.assign<Transform>(e1);
	registry.assign<Physics>(e1);
	registry.assign<Transform>(e2, glm::vec3(1, 1, 1));
	registry.assign<Name>(e3, "hello");

    bool contains1 = registry.contains(e1);
    registry.destroy(e1);
    contains1 = registry.contains(e1);
    registry.destroy(e2);
    registry.destroy(e3);
    registry.destroy(e4);
	Entity e5 = registry.create();
    contains1 = registry.contains(e1);
    Entity e6 = registry.create();
    contains1 = registry.contains(e1);
    Entity e7 = registry.create();
    

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
