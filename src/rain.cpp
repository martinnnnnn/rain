#include "rain.h"

#include <vector>
#include <string>
#include <windows.h>
#include <iterator>

#include "core/game_engine.h"


namespace rain
{
    int Rain::Init(std::unordered_map<std::string, std::string> _args)
    {
        return GameEngine::Get().Init(_args);
    }

    void Rain::Run()
    {
        GameEngine::Get().Run();
    }

    GameEngine* Rain::Engine()
    {
        return &(GameEngine::Get());
    }

    GLFWwindow* Rain::Window()
    {
        return GameEngine::Get().Getwindow();
    }

    Camera* Rain::Camera()
    {
        return GameEngine::Get().GetCamera();
    }

    InputEngine* Rain::Input()
    {
        return GameEngine::Get().GetInputEngine();
    }

    float Rain::GetDeltaTime()
    {
        return GameEngine::Get().GetDeltaTime();
    }

    std::string Rain::GetExePath()
    {
        char buffer[MAX_PATH];
        GetModuleFileName(NULL, buffer, MAX_PATH);
        std::string::size_type pos = std::string(buffer).find_last_of("\\/");
        return std::string(buffer).substr(0, pos);
    }

    std::string Rain::ResourcesRoot()
    {
        return GameEngine::Get().GetResourcesRoot();
    }

    glm::vec2 Rain::GetWindowSize()
    {
        return Engine()->GetWindowSize();
    }
    
    std::unordered_map<std::string, std::string> Rain::GetArguments(int _argc, char** _argv)
    {
        std::unordered_map<std::string, std::string> args;
        for (int i = 0; i < _argc; i++)
        {
            std::vector<std::string> first = Rain::split(_argv[i], ';');
            for (size_t j = 0; j < first.size(); j++)
            {
                std::vector<std::string> second = Rain::split(first[j], '=');
                if (second.size() > 1)
                {
                    args[second[0]] = second[1];
                }
            }
        }
        return args;
    }

    std::vector<std::string> Rain::split(const std::string &s, char delim)
    {
        std::vector<std::string> elems;
        split(s, delim, std::back_inserter(elems));
        return elems;
    }

}