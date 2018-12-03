#include "win32_application.h"

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
#include "core/logger.h"
#include "core/id_generator.h"

#include <boost/uuid/uuid.hpp>
#include <boost/uuid/name_generator.hpp>
#include <boost/uuid/uuid_io.hpp>

#include <rapidjson/document.h>
#include <rapidjson/filewritestream.h>
#include <rapidjson/writer.h>
#include <cstdio>

namespace rain
{


    int Application::init(HINSTANCE _hinstance, const std::string& _config)
    {
        hinstance = _hinstance;



        // INIT LOGGER
        logger = new Logger();

        id_generator = new IdGenerator();
        auto id1 = id_generator->get_new_unique_id();
        auto id2 = id_generator->get_new_unique_id();
        auto id3 = id_generator->get_new_unique_id();
        auto id4 = id_generator->get_new_unique_id();
        auto id5 = id_generator->get_new_unique_id("name 1");
        auto id6 = id_generator->get_new_unique_id("name 2");
        auto id7 = id_generator->get_new_unique_id("name 3");
        auto id8 = id_generator->get_new_unique_id("name 4");
        auto id9 = id_generator->get_new_unique_id("name 4");

        RAIN_LOG("rand %s", id_generator->str(id1).c_str());
        RAIN_LOG("rand %s", id_generator->str(id2).c_str());
        RAIN_LOG("rand %s", id_generator->str(id3).c_str());
        RAIN_LOG("rand %s", id_generator->str(id4).c_str());
        RAIN_LOG("name %s", id_generator->str(id5).c_str());
        RAIN_LOG("name %s", id_generator->str(id6).c_str());
        RAIN_LOG("name %s", id_generator->str(id7).c_str());
        RAIN_LOG("name %s", id_generator->str(id8).c_str());
        RAIN_LOG("name %s", id_generator->str(id9).c_str());

        // INIT CONFIG
        config = new Config();
        config->init(FilePath::get_exe_path() + "/config.rain");
        DataSystem data_system{ config->data_root };

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

        world = new World();
        world->init(RAIN_CONFIG->data_root + RAIN_CONFIG->starting_world);

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

            world->update_physics((float)dt);
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
        renderer->draw();

        //renderer.set_view_matrix(camera.position, glm::radians(camera.pitch), glm::radians(camera.yaw));
        //renderer->set_view_matrix(camera->position, camera->position + camera->front, camera->up);

        renderer->draw_text_2d("hello", 25.0f, 25.0f, 1.0f, glm::vec3(0.5, 0.8f, 0.2f));
        world->render(_alpha);


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

