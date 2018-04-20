#pragma once


#include <sstream>
#include <string>
#include <unordered_map>

class InputEngine;
class GameEngine;
class GLFWwindow;
class Camera;

class Rain
{
public:
    static void Init(std::unordered_map<std::string, std::string> _args);
    static void Run();

    static float GetDeltaTime();
    static GameEngine* Engine();
    static GLFWwindow* Window();
    static Camera* Camera();
    static InputEngine* Input();
    static std::string ResourcesRoot();
    static std::string GetExePath();

    static std::vector<std::string> split(const std::string &s, char delim);

    template<typename Out>
    static void split(const std::string &s, char delim, Out result)
    {
        std::stringstream ss(s);
        std::string item;
        while (std::getline(ss, item, delim))
        {
            *(result++) = item;
        }
    }
};
