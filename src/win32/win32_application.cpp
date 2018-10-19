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
    camera.movement_speed = 0.1f;
    camera.yaw = 0.0;
    camera.pitch = 0.0;
    camera.front = glm::vec3(0.0, 0.0, -1.0f);
    camera.right = glm::vec3(glm::cross(camera.front, glm::vec3(0.0, 1.0, 0.0)));
    camera.worldUp = glm::vec3(0.0f, 1.0f, 0.0f);

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

    GETINPUT.initialize();


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

void Application::shutdown()
{
    GETINPUT.shutdown();
}

void Application::update_camera()
{
    if (GETINPUT.is_key_pressed(DIK_P))
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

    // get camera
    
    glm::vec3 movement(0.0f, 0.0f, 0.0f);
    glm::vec3 front = camera.front;
    glm::vec3 right = camera.right;

    if (GETINPUT.is_key_pressed(DIK_W))
    {
        movement += front * camera.movement_speed;
    }
    if (GETINPUT.is_key_pressed(DIK_S))
    {
        movement -= front * camera.movement_speed;
    }
    if (GETINPUT.is_key_pressed(DIK_A))
    {
        movement -= right * camera.movement_speed;
    }
    if (GETINPUT.is_key_pressed(DIK_D))
    {
        movement += right * camera.movement_speed;
    }
    camera.position += movement;


    camera.yaw += (float)GETINPUT.x_offset * 0.1f;
    camera.pitch += (float)GETINPUT.y_offset * 0.1f;
    camera.pitch = std::clamp(camera.pitch, -89.0f, 89.0f);

    front.x = cos(glm::radians(camera.yaw)) * cos(glm::radians(camera.pitch));
    front.y = sin(glm::radians(camera.pitch));
    front.z = sin(glm::radians(camera.yaw)) * cos(glm::radians(camera.pitch));
    camera.front = glm::normalize(front);

    camera.right = glm::normalize(glm::cross(camera.front, camera.worldUp));
    camera.up = glm::normalize(glm::cross(camera.right, camera.front));
}


void Application::render()
{
    renderer->clear();
    renderer->set_view_matrix(camera.position, glm::radians(camera.pitch), glm::radians(camera.yaw));

    renderer->render_coord_view(glm::vec3(0.0f, 0.0f, 0.0f));


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
