#include "win32_application.h"

#include "win32_window.h"
#include "engine/win32/win32_input.h"
#include "engine/win32/win32_window.h"
#include "engine/core/event.h"
#include "engine/core/config.h"
#include "engine/math/transform.h"
#include "engine/gfx/ogl/ogl_renderer.h"
#include "engine/physics/physics.h"
#include "engine/physics/rigid_body.h"
#include "engine/physics/collision.h"
#include "engine/physics/spring.h"
#include "engine/game/world.h"
#include "engine/core/config.h"
#include "engine/core/logger.h"
#include "engine/core/id_generator.h"
#include "engine/data/data_system.h"
#include "engine/core/context.h"

// TEMP
#include "engine/network/client.h"


namespace rain::engine
{

    int Application::init(HINSTANCE _hinstance, const std::string& _config)
    {
        hinstance = _hinstance;

        Context* engine_context = Context::get();

        // INIT LOGGER
        engine_context->logger = new Logger();

        // INIT ID GENERATOR
        engine_context->id_generator = new IdGenerator();

        // INIT CONFIG
        engine_context->config = new Config();
        engine_context->config->init(Win32::get_exe_path() + "/config.rain");
        
        // TEMP
        //core::FilePathSystem file_system;
        //file_system.init(RAIN_CONFIG->data_root);

        // LOADING DATA
        engine_context->data_system = new DataSystem();
        engine_context->data_system->load_all_recursive(RAIN_CONFIG->data_root);

        //client = new Client();
        //init_socket(client);
        
	    // INIT WINDOW
        engine_context->window = new Window();
	    engine_context->window->init(hinstance, RAIN_CONFIG->screen_width, RAIN_CONFIG->screen_height, 0);

        // INIT RENDERER
        engine_context->renderer = new Renderer();
	    engine_context->renderer->init();

        // SHOW WINDOW
        engine_context->window->show();

	    // INIT INPUT
        engine_context->input = new Input();
	    engine_context->input->init();

        // INIT CLOCK
        engine_context->clock = new HighResolutionClock();
        engine_context->clock->reset();

        world = new World();
        world->init(RAIN_CONFIG->data_root + RAIN_CONFIG->starting_world);

        return 0;
    }


    void Application::update()
    {
        static const double dt = 0.01;

        static double currentTime = RAIN_CLOCK->get_total_seconds();
        static double accumulator = 0.0;

        RAIN_INPUT->update();
        RAIN_RENDERER->update_camera();
        RAIN_CLOCK->tick();

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
            accumulator -= dt;
        }

        const double alpha = accumulator / dt;
        render(float(alpha));
        if (RAIN_WINDOW->is_initialized())
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
        RAIN_RENDERER->clear();
        RAIN_RENDERER->draw();

        //renderer.set_view_matrix(camera.position, glm::radians(camera.pitch), glm::radians(camera.yaw));
        //renderer->set_view_matrix(camera->position, camera->position + camera->front, camera->up);

        RAIN_RENDERER->draw_text_2d("hello", 25.0f, 25.0f, 1.0f, glm::vec3(0.5, 0.8f, 0.2f));
        world->render(_alpha);
    }
}





int main(int argc, char* argv[])
{
   

    return 0;
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
        if (PeekMessage(&msg, NULL, NULL, NULL, PM_REMOVE))
        {
            if (msg.message == WM_QUIT)
                quit = true;

            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }

        application.update();

        if (GetAsyncKeyState(VK_ESCAPE))
        {
            RAIN_WINDOW->shutdown();
        }
    }

    return msg.lParam;
}

