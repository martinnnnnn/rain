#pragma once



#include <sstream>
#include <string>
#include <unordered_map>
#include <functional>

#include "graphics/graphics.h"

namespace rain
{

    struct Input;
    class CameraController;

    struct Game
    {
        std::string dataPath;
        GFXContext gfxContext;
        Input* input;
        CameraController* cameraController;
        float deltaTime;
        float lastFrame;
    };


        Game InitGame(const std::string& _dataPath);
        Game* InitGame(int argc, char** argv);
        void RunGame(Game* _game, std::function<void(void)> _updateCallBack);
}
