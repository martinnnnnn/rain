#pragma once

#include "utility/incl_3d.h"


#include <sstream>
#include <string>
#include <unordered_map>

namespace rain
{
    class InputEngine;
    class GameEngine;
    class Camera;

    class Rain
    {
    public:
        static int Init(std::unordered_map<std::string, std::string> _args);
        static void Run();

        static float GetDeltaTime();
        static GameEngine* Engine();
        static GLFWwindow* Window();
        static Camera* Camera();
        static InputEngine* Input();
        static std::string ResourcesRoot();
        static std::string GetExePath();
        static glm::vec2 GetWindowSize();

        // utility functions
        static std::unordered_map<std::string, std::string> GetArguments(int argc, char** argv);
    };
}
