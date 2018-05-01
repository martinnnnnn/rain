#include "game_engine.h"


#include <iostream>

#include "utility/stdincl_3d.h"

#include "rain.h"
#include "camera_controller.h"
#include "graphics/camera.h"
#include "input/input_engine.h"

namespace rain
{
    //----------------------------------------------------------
    GameEngine::GameEngine() :
        m_inputEngine(nullptr),
        m_window(nullptr)
    {

    }

    int GameEngine::Init(std::unordered_map<std::string, std::string> _args)
    {
        m_resourcesRootPath = Rain::GetExePath();
        if (_args["root"] != "")
        {
            m_resourcesRootPath = _args["root"];
        }
        std::cout << "root path : " << m_resourcesRootPath.c_str() << std::endl;

        m_screenWidth = 800;
        m_screenHeight = 600;

        glfwInit();
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        m_window = glfwCreateWindow((int)m_screenWidth, (int)m_screenHeight, "Rain Engine", NULL, NULL);
        if (m_window == NULL)
        {
            std::cout << "Failed to create GLFW window" << std::endl;
            glfwTerminate();
            return -1;
        }
        //glfwSetWindowPos(m_window, 600, 600);
        glfwMakeContextCurrent(m_window);

        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
        {
            std::cout << "Failed to initialize GLAD" << std::endl;
            return -1;
        }

        glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LESS);
		//glEnable(GL_CULL_FACE);
		//glFrontFace(GL_CW);
        glViewport(0, 0, (GLsizei)m_screenWidth, (GLsizei)m_screenHeight);
        glfwSetFramebufferSizeCallback(m_window, GameEngine::framebuffer_size_callback);

        glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        glfwSetInputMode(m_window, GLFW_STICKY_KEYS, 1);

        m_inputEngine = new InputEngine(m_window);
        m_cameraController = new CameraController();
        m_cameraController->Init(m_inputEngine);
        return 0;
    }

    void GameEngine::SetUpdateCallback(std::function<void(void)> _function)
    {
        m_updateCallback = _function;
    }

    void GameEngine::Run()
    {
        while (!glfwWindowShouldClose(m_window))
        {
            // inputs relative to window
            if (m_inputEngine->IsKeyPressed(GLFW_KEY_ESCAPE))
            {
                glfwSetWindowShouldClose(m_window, true);
            }

            // delta time update
            float currentFrame = (float)glfwGetTime();
            m_deltaTime = currentFrame - m_lastFrame;
            m_lastFrame = currentFrame;

            // input
            m_inputEngine->Tick();

            // objects
            m_cameraController->Tick();
            GetCamera()->Tick();

            glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
			glEnable(GL_STENCIL_TEST);
			glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
            // rendering
            m_updateCallback();



            glfwSwapBuffers(m_window);
            glfwPollEvents();
        }

        glfwTerminate();
    }

    GLFWwindow* GameEngine::Getwindow()
    {
        return m_window;
    }

    CameraController* GameEngine::GetCameraController()
    {
        return m_cameraController;
    }

    Camera* GameEngine::GetCamera()
    {
        return m_cameraController->GetCamera();
    }

    InputEngine* GameEngine::GetInputEngine()
    {
        return m_inputEngine;
    }

    std::string GameEngine::GetResourcesRoot()
    {
        return m_resourcesRootPath;
    }

    float GameEngine::GetDeltaTime()
    {
        return m_deltaTime;
    }

    void GameEngine::framebuffer_size_callback(::GLFWwindow* window, int width, int height)
    {
        glViewport(0, 0, width, height);

    }
}