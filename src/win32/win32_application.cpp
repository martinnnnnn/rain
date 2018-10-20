#include "win32_application.h"

#include <windows.h>

#include <time.h>
#include <stdlib.h>
#include <algorithm>

#include "win32_window.h"
#include "gfx/ogl/ogl_renderer.h"
#include "win32/win32_input.h"
#include "core/event.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

int Application::init(HINSTANCE _hinstance, const std::string& _config)
{
    hinstance = _hinstance;

	// INIT WINDOW
	RAIN_WINDOW.init(hinstance, 800, 600, 0);
	renderer.init();
	RAIN_WINDOW.show();

	// INIT INPUT
	RAIN_INPUT.init();


	camera.init();


	auto entity = registry.create();
	registry.assign<Transform>(entity, 0.0f, 0.0f, 0.0f);

    //srand(time(NULL));
    //for (auto i = 0; i < 50; ++i)
    //{
    //    auto entity = registry.create();

    //    registry.assign<Transform>(entity, glm::vec3(i * 1.f, i * 1.f, i * 1.f));
    //    
    //    float a = (rand() % 100) / 100.0;
    //    float b = 1.0 - a;
    //    registry.assign<Physics>(entity, glm::vec3(a, b, 0), i * .1f);
    //}

    return 0;
}

void Application::update()
{
	if (RAIN_INPUT.is_key_pressed(DIK_P))
	{
		static bool cursor_visible = true;
		cursor_visible = !cursor_visible;
		if (cursor_visible)
		{
			::ShowCursor(true);
		}
		else
		{
			::ShowCursor(false);
		}
	}
	
	camera.update();

    //auto view = registry.view<Transform, Physics>();

    //char buffer[512];
    //sprintf_s(buffer, "\n\n");
    //OutputDebugStringA(buffer);

    //for (auto entity : view)
    //{
    //    Physics& physics = view.get<Physics>(entity);
    //    Transform& transform = view.get<Transform>(entity);
    //    
    //    sprintf_s(buffer, "(%f,%f)\n", transform.position.x, transform.position.y);
    //    OutputDebugStringA(buffer);

    //    transform.position.x += physics.direction.x * physics.speed;
    //    transform.position.y += physics.direction.y * physics.speed;


    //    if (transform.position.x <= 0 || transform.position.x >= 800)
    //    {
    //        transform.position.x = std::clamp(transform.position.x, 0.0f, 800.0f);
    //        physics.direction.x = -physics.direction.x;
    //    }
    //    if (transform.position.y <= 0 || transform.position.y >= 600)
    //    {
    //        transform.position.y = std::clamp(transform.position.y, 0.0f, 600.0f);
    //        physics.direction.y = -physics.direction.y;
    //    }
    //}
}

void Application::shutdown()
{
    RAIN_INPUT.shutdown();
}


void Application::render()
{
    renderer.clear();
    //renderer.set_view_matrix(camera.position, glm::radians(camera.pitch), glm::radians(camera.yaw));
    renderer.set_view_matrix(camera.position, camera.position + camera.front, camera.up);
    renderer.render_coord_view(glm::vec3(0.0f, 0.0f, 0.0f));

    auto view = registry.view<Transform>();

    for (auto entity : view)
    {
        Transform& transform = view.get(entity);
        renderer.render_cube(transform.position);
    }
}


int WINAPI WinMain(HINSTANCE hinstance, HINSTANCE hprevinstance, LPSTR lpcmdline, int nshowcmd)
{
	Application app;
    app.init(hinstance, "");

	MSG msg;
	bool quit = false;
	while (!quit)
	{
        RAIN_INPUT.update();

        if (PeekMessage(&msg, NULL, NULL, NULL, PM_REMOVE))
        {
            if (msg.message == WM_QUIT)
                quit = true;

            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }

        app.update();

		if (RAIN_WINDOW.is_initialized())
		{
			app.render();
			RAIN_WINDOW.present();
		}

        if (GetAsyncKeyState(VK_ESCAPE))
        {
            RAIN_WINDOW.shutdown();
        }
	}

	return msg.lParam;
}
