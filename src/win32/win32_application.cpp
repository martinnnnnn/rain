#include "win32_application.h"

#include <windows.h>

#include <time.h>
#include <stdlib.h>
#include <algorithm>

#include "win32_window.h"
#include "ogl/ogl_renderer.h"



int Application::init(HINSTANCE _hinstance, const std::string& _config)
{
    hinstance = _hinstance;


    auto camera = registry.create();
    registry.assign<Camera>(camera);



    srand(time(NULL));

    //for (auto i = 0; i < 50; ++i)
    //{
    //    auto entity = registry.create();

    //    registry.assign<Transform>(entity, glm::vec3(i * 1.f, i * 1.f, i * 1.f));
    //    
    //    float a = (rand() % 100) / 100.0;
    //    float b = 1.0 - a;
    //    registry.assign<Physics>(entity, glm::vec3(a, b, 0), i * .1f);
    //}

    auto entity = registry.create();

    registry.assign<Transform>(entity, 1.f, 1.f, 0.0f);

    float a = (float)((rand() % 100) / 100.0);
    float b = (float)(1.0 - a);
    registry.assign<Physics>(entity, a, b, 0.0f, 2.0f);


    GETWINDOW.initialize(hinstance, 800, 600, 0);

    renderer = new Renderer();
    renderer->initialize();
    GETWINDOW.show();

    return 0;
}

void Application::update()
{
    auto view = registry.view<Transform, Physics>();

    char buffer[512];
    sprintf_s(buffer, "\n\n");
    OutputDebugStringA(buffer);

    for (auto entity : view)
    {
        Physics& physics = view.get<Physics>(entity);
        Transform& transform = view.get<Transform>(entity);
        
        sprintf_s(buffer, "(%f,%f)\n", transform.position.x, transform.position.y);
        OutputDebugStringA(buffer);

        transform.position.x += physics.direction.x * physics.speed;
        transform.position.y += physics.direction.y * physics.speed;


        if (transform.position.x <= 0 || transform.position.x >= 800)
        {
            transform.position.x = std::clamp(transform.position.x, 0.0f, 800.0f);
            physics.direction.x = -physics.direction.x;
        }
        if (transform.position.y <= 0 || transform.position.y >= 600)
        {
            transform.position.y = std::clamp(transform.position.y, 0.0f, 600.0f);
            physics.direction.y = -physics.direction.y;
        }
    }
}

void Application::render()
{
    auto view = registry.view<Transform, Physics>();

    for (auto entity : view)
    {
        Transform& transform = view.get<Transform>(entity);
        //renderer->render_quad(transform.position);
    }
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
