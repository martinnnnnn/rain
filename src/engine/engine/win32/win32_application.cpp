#include "win32_application.h"

#include "math/math.h"
#include "win32_window.h"
#include "engine/win32/win32_input.h"
#include "engine/win32/win32_window.h"
#include "engine/core/event.h"
#include "engine/core/config.h"
#include "engine/gfx/ogl/ogl_renderer.h"
#include "engine/game/world.h"
#include "engine/core/config.h"
#include "engine/data/data_system.h"
#include "engine/core/context.h"
#include "engine/core/profiler.h"
// TEMP
#include "engine/network/network.h"
#include "engine/network/server.h"
#include <filesystem>

namespace rain::engine
{

    void print_to_log(const char* _msg)
    {
        OutputDebugStringA(_msg);
    }

    int Application::init(void* _hinstance, const std::string& _config)
    {
        hinstance = _hinstance;

        // INIT LOGGER
        RAIN_CONTEXT->logger = new core::Logger(print_to_log);

        // INIT CONFIG
        RAIN_CONFIG = new Config();
        RAIN_CONFIG->init(Win32::get_exe_path() + RAIN_CONFIG_FILE_NAME);
        
        // TEMP
        network::init();
        start_tcp_client("127.0.0.1", "9999");
        //network::start_server("127.0.0.1", "9998");
        network::start_server_thread("127.0.0.1:9998");
        //network::terminate();
        //core::FilePathSystem file_system;
        //file_system.init(RAIN_CONFIG->data_root);
		UI.init();
        
	    // INIT WINDOW
        RAIN_CONTEXT->window = new Window();
	    RAIN_CONTEXT->window->init((HINSTANCE)hinstance, RAIN_CONFIG->screen_width, RAIN_CONFIG->screen_height, 0);

        // INIT RENDERER
        RAIN_CONTEXT->renderer = new Renderer();
	    RAIN_CONTEXT->renderer->init();

        // SHOW WINDOW
        RAIN_CONTEXT->window->show();

        // LOADING DATA
        RAIN_CONTEXT->data_sys = new data_system();
        RAIN_CONTEXT->data_sys->load_all_recursive(RAIN_CONFIG->data_root);

	    // INIT INPUT
        RAIN_CONTEXT->input = new Input();
	    RAIN_CONTEXT->input->init();

        // INIT CLOCK
        RAIN_CONTEXT->clock = new HighResolutionClock();
        RAIN_CONTEXT->clock->reset();

        world = new World();
        world->init(RAIN_CONFIG->data_root + RAIN_CONFIG->starting_world);

        return 0;
    }


    void Application::update()
    {
        RAIN_RENDERER->clear();

        static const f64 dt = 0.01;

        static f64 currentTime = RAIN_CLOCK->get_total_seconds();
        static f64 accumulator = 0.0;

        RAIN_INPUT->update();
        RAIN_CLOCK->tick();

		UI.update();

        double newTime = RAIN_CLOCK->get_total_seconds();
        double frameTime = newTime - currentTime;
        if (frameTime > 0.25)
        {
            frameTime = 0.25;
        }
        currentTime = newTime;
        accumulator += frameTime;

        while (accumulator >= dt)
        {
            world->update_physics((float)dt);
            world->update_camera((float)dt);
            accumulator -= dt;
        }

        const double alpha = accumulator / dt;
        render(float(alpha));

        if (RAIN_WINDOW->initialized)
        {
            RAIN_WINDOW->present();
        }

    }

    void Application::shutdown()
    {
        RAIN_INPUT->shutdown();
    }


    void Application::render(float _alpha)
    {
        RAIN_RENDERER->draw();
        //renderer.set_view_matrix(camera.position, glm::radians(camera.pitch), glm::radians(camera.yaw));
        //renderer->set_view_matrix(camera->position, camera->position + camera->front, camera->up);

        world->render(_alpha);
    }
}


int WINAPI WinMain(HINSTANCE hinstance, HINSTANCE hprevinstance, LPSTR lpcmdline, int nshowcmd)
{
    rain::engine::Application application;
    
    if (application.init(hinstance, "") != 0)
    {
        return -1;
    }

    MSG msg;
    bool quit = false;
    while (!quit)
    {
        if (::PeekMessage(&msg, NULL, NULL, NULL, PM_REMOVE))
        {
            if (msg.message == WM_QUIT)
                quit = true;

            ::TranslateMessage(&msg);
            ::DispatchMessage(&msg);
        }

        application.update();

        if (::GetAsyncKeyState(VK_ESCAPE))
        {
            RAIN_WINDOW->shutdown();
        }
    }

    return msg.lParam;
}

