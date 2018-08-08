#pragma once



#include <sstream>
#include <string>
#include <unordered_map>
#include <functional>

#include "gfx/gfx_graphics.h"

namespace rain
{

    struct Input;
    struct Camera;

    struct Game
    {
        std::string dataPath;
        GFXContext gfxContext;
        Input* input;
        Camera* camera;
        float deltaTime;
        float lastFrame;
    };

    Game* CreateGame(const std::string& _dataPath);
    Game* CreateGame(int _argc, char** _argv);
    void RunGame(Game* _game, std::function<void(void)> _updateCallBack);
    void InitUI(GLFWwindow* _window);
    void UpdateUI();
}
