#pragma once

#include <Windows.h>



#include <string>
#include <unordered_map>
#include <vector>
#include <functional>
#include <cassert>

#include <glm/glm.hpp>
#include <entt.hpp>

#include "core/singleton.h"
#include "core/types.h"
#include "gfx/gfx_camera.h"
#include "core/high_resolution_clock.h"

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
    class Data;
    class Renderer;
    class Input;

    class Application : public Singleton<Application>
    {
    public:
        int init(HINSTANCE _hinstance, const std::string& _config);
        void shutdown();
        void update();
        void update_physics(float _deltaTime);
        void render(float _alpha);

        Data* data;
        Window* window;
        Renderer* renderer;
        Input* input;

    private:
        HINSTANCE hinstance;
        entt::DefaultRegistry registry;
        Camera camera;
        HighResolutionClock m_clock;
    };
}

#define RAIN_APPLICATION rain::Application::Get()
#define RAIN_DATA rain::Application::Get().data
#define RAIN_WINDOW rain::Application::Get().window
#define RAIN_INPUT rain::Application::Get().input
