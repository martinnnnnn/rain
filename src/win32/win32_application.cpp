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

#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>
#include <time.h>
#include <stdio.h>  
#include <fcntl.h>  
#include <string.h>  
#include <io.h>

#include "physics/physics.h"
#include "math/transform.h"

using namespace rain;


int Application::init(HINSTANCE _hinstance, const std::string& _config)
{
    hinstance = _hinstance;

	// INIT WINDOW
	RAIN_WINDOW.init(hinstance, 800, 600, 0);
	renderer.init();
	RAIN_WINDOW.show();

	// INIT INPUT
	RAIN_INPUT.init();

    // INIT CAMERA
	camera.init();

    // INIT CLOCK
    m_clock.reset();

    // ADDING A FEW ENTITIES
    auto entity = registry.create();
    registry.assign<Transform>(entity);
    registry.assign<RigidBody>(entity);

    auto entity2 = registry.create();
    Transform& transform2 = registry.assign<Transform>(entity2);
    transform2.currentPosition = glm::vec3(0.0f, 12.0f, 0.0f);
    transform2.previousPosition = glm::vec3(0.0f, 12.0f, 0.0f);
    
    registry.assign<RigidBody>(entity2);

    Physics::init(registry);

    return 0;
}


void Application::update()
{
    static double t = 0.0;
    static double dt = 0.01;

    static double currentTime = m_clock.get_total_seconds();
    static double accumulator = 0.0;

    RAIN_INPUT.update();
    camera.update();
    m_clock.tick();

    double newTime = m_clock.get_total_seconds();
    double frameTime = newTime - currentTime;
    if (frameTime > 0.25)
    {
        frameTime = 0.25;
    }
    currentTime = newTime;
    accumulator += frameTime;

    while (accumulator >= dt)
    {

        update_physics((float)dt);
        accumulator -= dt;
    }

    const double alpha = accumulator / dt;
    render(alpha);
    if (RAIN_WINDOW.is_initialized())
    {
        RAIN_WINDOW.present();
    }
}

void Application::update_physics(float _deltaTime)
{
    Physics::update(registry, _deltaTime);


    //auto view = registry.view<Transform, RigidBody>();
    //for (auto entity : view)
    //{
    //    Physics& physics = view.get<Physics>(entity);
    //    Transform& transform = view.get<Transform>(entity);

    //    transform.previousPosition = transform.currentPosition;

    //    physics.velocity += (physics.force / physics.mass) * _deltaTime;
    //    transform.currentPosition += physics.velocity * _deltaTime;
    //}
}


void Application::shutdown()
{
    RAIN_INPUT.shutdown();
}


void Application::render(float _alpha)
{
    renderer.clear();
    //renderer.set_view_matrix(camera.position, glm::radians(camera.pitch), glm::radians(camera.yaw));
    renderer.set_view_matrix(camera.position, camera.position + camera.front, camera.up);
    renderer.render_coord_view(glm::vec3(0.0f, 0.0f, 0.0f));

    auto view = registry.view<Transform>();

    for (auto entity : view)
    {
        Transform& transform = view.get(entity);
        glm::vec3 position = transform.currentPosition * _alpha + transform.previousPosition * (1.0f - _alpha);
        renderer.render_cube(position);
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
        if (PeekMessage(&msg, NULL, NULL, NULL, PM_REMOVE))
        {
            if (msg.message == WM_QUIT)
                quit = true;

            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }

        app.update();

        if (GetAsyncKeyState(VK_ESCAPE))
        {
            RAIN_WINDOW.shutdown();
        }
    }

    return msg.lParam;
}
