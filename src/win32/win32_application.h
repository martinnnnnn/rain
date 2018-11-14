#pragma once

#include <Windows.h>
#include <string>
#include <unordered_map>
#include <vector>
#include <functional>
#include <cassert>

#include <glm/glm.hpp>
//#include <entt.hpp>

#include "core/singleton.h"
#include "core/types.h"
//#include "gfx/gfx_camera.h"
#include "core/high_resolution_clock.h"
#include "game/world.h"

namespace rain
{
	struct Name
	{
		Name() : Name("default") {}

		Name(const std::string& _name) :
			name(_name)
		{
		}

		std::string name;
	};


    class Window;
    struct Config;
    class Renderer;
    class Input;

    class Application : public Singleton<Application>
    {
    public:

        int init(HINSTANCE _hinstance, const std::string& _config);
        void shutdown();
        void update();
        void render(float _alpha);

        Config* config;
        Window* window;
        Renderer* renderer;
        Input* input;

    private:
        HINSTANCE hinstance;
        World world;
        HighResolutionClock m_clock;
    };
}

#define RAIN_APPLICATION rain::Application::get()
#define RAIN_WINDOW rain::Application::get().window
#define RAIN_INPUT rain::Application::get().input
