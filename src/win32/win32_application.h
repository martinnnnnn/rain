#pragma once

#include <string>
#include <unordered_map>
#include <vector>
#include <functional>
#include <cassert>

#include <glm/glm.hpp>

#include "win32/win32_helper.h"
#include "core/singleton.h"
#include "core/types.h"
#include "core/high_resolution_clock.h"
#include "game/world.h"

namespace rain
{





    class Window;
    struct Config;
    class Renderer;
    class Input;
    class Logger;

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
        Logger* logger;

    private:
        HINSTANCE hinstance;
        World world;
        HighResolutionClock m_clock;
    };
}

#define RAIN_APPLICATION rain::Application::get()
