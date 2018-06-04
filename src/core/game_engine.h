#pragma once

#include <glm/glm.hpp>

#include <unordered_map>
#include <string>
#include <functional>

#include "imgui.h"
#include "graphics/graphics.h"


struct GLFWwindow;

namespace rain
{
    struct Input;
    class CameraController;
    class Camera;
    class InputEngine;
	class FileSystem;

    class GameEngine
    {
    public:

        int Init(const std::string& _path);
        int Init(std::unordered_map<std::string, std::string> _args);
        void Run();
        
        GLFWwindow* Getwindow();
        InputEngine* GetInputEngine();
        CameraController* GetCameraController();
        Camera* GetCamera();
        float GetDeltaTime();
        std::string GetResourcesRoot();
        void SetResourcesRoot(const std::string& _path);
        void SetUpdateCallback(std::function<void(void)> _function);
        glm::vec2 GetWindowSize();
    private:
        InputEngine* m_inputEngine;
        GLFWwindow* m_window;
        CameraController* m_cameraController;
		FileSystem* m_fileSystem;

        float m_deltaTime;
        float m_screenWidth;
        float m_screenHeight;
        std::string m_resourcesRootPath;
        float m_lastFrame;
        std::function<void(void)> m_updateCallback;

		bool show_demo_window;
		bool show_another_window;
		ImVec4 clear_color;
    private:
        static void framebuffer_size_callback(GLFWwindow* window, int width, int height);
		void initUI();
		void updateUI();
		

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


    struct Game
    {
        std::string dataPath;
        GFXContext gfxContext;
        //InputEngine* inputEngine;
        Input* input;
        CameraController* cameraController;
        float deltaTime;
        float lastFrame;
    };


    Game InitGame(const std::string& _dataPath);
    Game* InitGame(int argc, char** argv);
    void RunGame(Game* _game, std::function<void(void)> _updateCallBack);

}
