#pragma once

#include <string>
#include <unordered_map>
#include <vector>
#include <functional>

#include "core/core.h"
#include "engine/win32/win32_helpers.h"

namespace rain::engine
{
    struct World;

    class Application
    {
    public:

        int init(HINSTANCE _hinstance, const std::string& _config);
        void shutdown();
        void update();
        void render(float _alpha);

    private:
        World* world;
        HINSTANCE hinstance;
    };
}