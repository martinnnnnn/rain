#include "win32_application.h"

#include <entt.hpp>
#include <windows.h>
#include <time.h>
#include <stdlib.h>
#include <algorithm>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>

#include "win32_window.h"
#include "win32/win32_input.h"
#include "win32/win32_window.h"
#include "core/event.h"
#include "core/data_indexer.h"
#include "math/transform.h"
#include "gfx/ogl/ogl_renderer.h"
#include "gfx/ogl/ogl_renderer.h"
#include "physics/physics.h"

using namespace rain;


int Application::init(HINSTANCE _hinstance, const std::string& _config)
{
    hinstance = _hinstance;

    // INIT CONFIG
    data = new Data();
    data->init(File::get_exe_path() + "/config.rain");

	// INIT WINDOW
    window = new Window();
	window->init(hinstance, 800, 600, 0);

    // INIT RENDERER
    renderer = new Renderer();
	renderer->init();

    // SHOW WINDOW
	window->show();

	// INIT INPUT
    input = new Input();
	input->init();

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
    transform2.currentOrientation = glm::quat(glm::vec3(0, 0, 0)) * glm::angleAxis(glm::radians(30.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    transform2.previousOrientation = glm::quat(glm::vec3(0, 0, 0)) * glm::angleAxis(glm::radians(30.0f), glm::vec3(1.0f, 0.0f, 0.0f));

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

    input->update();
    renderer->update_camera();
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
    render(float(alpha));
    if (window->is_initialized())
    {
        window->present();
    }
}

void Application::update_physics(float _deltaTime)
{
    Physics::update(registry, _deltaTime);
}


void Application::shutdown()
{
    input->shutdown();
}


void Application::render(float _alpha)
{
    renderer->clear();
    //renderer.set_view_matrix(camera.position, glm::radians(camera.pitch), glm::radians(camera.yaw));
    //renderer->set_view_matrix(camera->position, camera->position + camera->front, camera->up);
    renderer->render_coord_view(glm::vec3(0.0f, 0.0f, 0.0f));

    auto view = registry.view<Transform>();

    for (auto entity : view)
    {
        Transform& transform = view.get(entity);
        glm::vec3 position = transform.currentPosition * _alpha + transform.previousPosition * (1.0f - _alpha);
        glm::quat orientation = transform.currentOrientation * _alpha + transform.previousOrientation * (1.0f - _alpha);

        renderer->render_cube(position, orientation);
    }
}


int WINAPI WinMain(HINSTANCE hinstance, HINSTANCE hprevinstance, LPSTR lpcmdline, int nshowcmd)
{
    RAIN_APPLICATION.init(hinstance, "");

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

        RAIN_APPLICATION.update();

        if (GetAsyncKeyState(VK_ESCAPE))
        {
            RAIN_WINDOW->shutdown();
        }
    }

    return msg.lParam;
}
