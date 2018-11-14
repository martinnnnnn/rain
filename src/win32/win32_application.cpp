#include "win32_application.h"

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
#include "core/config.h"
#include "math/transform.h"
#include "gfx/ogl/ogl_renderer.h"
#include "physics/physics.h"
#include "physics/rigid_body.h"
#include "physics/collision.h"
#include "physics/spring.h"
#include "game/world.h"
#include "core/config.h"

using namespace rain;


int Application::init(HINSTANCE _hinstance, const std::string& _config)
{
    hinstance = _hinstance;

    // INIT CONFIG
    config = new Config();
    config->init(File::get_exe_path() + "/config.rain");

	// INIT WINDOW
    window = new Window();
	window->init(hinstance, config->screen_width, config->screen_height, 0);

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

    world.init(RAIN_CONFIG->data_root + "/world/world_1.rain");

    return 0;
}


void Application::update()
{
    static const double dt = 0.01;

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

        world.update_physics((float)dt);
        accumulator -= dt;
    }

    const double alpha = accumulator / dt;
    render(float(alpha));
    if (window->is_initialized())
    {
        window->present();
    }
}

void Application::shutdown()
{
    input->shutdown();
}


void Application::render(float _alpha)
{
    renderer->clear();
    renderer->render_coord_view(glm::vec3(0.0f, 0.0f, 0.0f));
    //renderer.set_view_matrix(camera.position, glm::radians(camera.pitch), glm::radians(camera.yaw));
    //renderer->set_view_matrix(camera->position, camera->position + camera->front, camera->up);

    world.render(_alpha);
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
