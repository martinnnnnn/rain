#pragma once

#include <glm/glm.hpp>

#include <unordered_map>
#include <string>
#include <functional>
struct GLFWwindow;

namespace rain
{
    class CameraController;
    class Camera;
    class InputEngine;
    

    class GameEngine
    {
    public:

        int Init(std::unordered_map<std::string, std::string> _args);
        void Run();
        
        GLFWwindow* Getwindow();
        InputEngine* GetInputEngine();
        CameraController* GetCameraController();
        Camera* GetCamera();
        float GetDeltaTime();
        std::string GetResourcesRoot();
        void SetUpdateCallback(std::function<void(void)> _function);
        glm::vec2 GetWindowSize();
    private:
        InputEngine* m_inputEngine;
        GLFWwindow* m_window;
        CameraController* m_cameraController;

        float m_deltaTime;
        float m_screenWidth;
        float m_screenHeight;
        std::string m_resourcesRootPath;
        float m_lastFrame;
        std::function<void(void)> m_updateCallback;

    private:
        static void framebuffer_size_callback(GLFWwindow* window, int width, int height);

    public:
        ~GameEngine() {}
        static GameEngine& Get()
        {
            static GameEngine *instance = new GameEngine();
            return *instance;
        }
    private:
        GameEngine();
        GameEngine(GameEngine const&) = delete;
        void operator=(GameEngine const&) = delete;

    };
}
