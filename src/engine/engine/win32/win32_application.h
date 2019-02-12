#pragma once

#include <string>
#include <unordered_map>
#include <vector>
#include <functional>

#include "core/core.h"

#include "engine/ui/text_field.h"

namespace rain::engine
{
    struct World;

    class Application
    {
    public:

        int init(void* _hinstance, const std::string& _config);
        void shutdown();
        void update();
        void render(float _alpha);

    private:
        World* world;
        void* hinstance;
		ui::ui UI;
    };
}