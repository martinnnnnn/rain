#include "win32_application.h"

#include <windows.h>

#include <time.h>
#include <stdlib.h>
#include <algorithm>
#include <algorithm>

#include "win32_window.h"
#include "ogl/ogl_renderer.h"
#include "core/input.h"
#include "core/event.h"


int Application::init(HINSTANCE _hinstance, const std::string& _config)
{
    hinstance = _hinstance;

    //auto camera = registry.create();
    //registry.assign<Camera>(camera);

    camera.position = glm::vec3(0.0, 0.0, 1.0f);
    camera.movement_speed = 5;
    camera.yaw = 0.0;
    camera.pitch = 0.0;
    camera.front = glm::vec3(0.0, 0.0, -1.0f);
    camera.right = glm::vec3(glm::cross(camera.front, glm::vec3(0.0, 1.0, 0.0)));
 

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

    registry.assign<Transform>(entity, 0.0f, 0.0f, 0.0f);


    GETWINDOW.initialize(hinstance, 800, 600, 0);

    renderer = new Renderer();
    renderer->initialize();
    
    GETWINDOW.show();

    return 0;
}

void Application::update()
{
    
    update_camera();

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

void Application::update_camera()
{
    // get camera
    char buffer[512];
    
    glm::vec3 movement(0.0f, 0.0f, 0.0f);
    glm::vec3 front = camera.front;
    glm::vec3 right = camera.right;

    if (GETINPUT.get_input(KeyCode::Z))
    {
        movement += front * camera.movement_speed;
        sprintf_s(buffer, "Z\n");
        OutputDebugStringA(buffer);
    }
    if (GETINPUT.get_input(KeyCode::S))
    {
        sprintf_s(buffer, "S\n");
        OutputDebugStringA(buffer);
        movement -= front * camera.movement_speed;
    }
    if (GETINPUT.get_input(KeyCode::Q))
    {
        sprintf_s(buffer, "Q\n");
        movement -= right * camera.movement_speed;
    }
    if (GETINPUT.get_input(KeyCode::D))
    {
        sprintf_s(buffer, "D\n");
        OutputDebugStringA(buffer);
        movement += right * camera.movement_speed;
    }
    camera.position += movement;

    float mouseOffsetX = (float)GETINPUT.offsetX * 0.1f;
    float mouseOffsetY = (float)GETINPUT.offsetY * 0.1f;

    //if (mouseOffsetX != 0)
    //{
    //    sprintf_s(buffer, "x : %f\n", mouseOffsetX);
    //    OutputDebugStringA(buffer);
    //}
    //if (mouseOffsetY != 0)
    //{
    //    sprintf_s(buffer, "y : %f\n", mouseOffsetY);
    //    OutputDebugStringA(buffer);
    //}

    camera.yaw += mouseOffsetX * 0.1f;
    camera.pitch += mouseOffsetY * 0.1f;
    camera.pitch = std::clamp(camera.pitch, -89.0f, 89.0f);
}


void Application::render()
{
    renderer->clear();
    renderer->set_view_matrix(camera.position, glm::radians(camera.pitch), glm::radians(camera.yaw));

    auto view = registry.view<Transform>();

    for (auto entity : view)
    {
        Transform& transform = view.get(entity);
        renderer->render_cube(transform.position);
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
        GETINPUT.update();
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
