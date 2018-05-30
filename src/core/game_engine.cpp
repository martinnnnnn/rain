#include "game_engine.h"


#include <iostream>
#include <filesystem>
using namespace std::experimental::filesystem::v1;

#include "imgui.h"
#include "imgui_impl_glfw_gl3.h"

#include "utility/incl_3d.h"
#include "rain.h"
#include "camera_controller.h"
#include "graphics/camera.h"
#include "input/input_engine.h"
#include "file_system.h"

namespace rain
{
    //----------------------------------------------------------
    GameEngine::GameEngine() :
        m_inputEngine(nullptr),
        m_window(nullptr)
    {

    }

    int GameEngine::Init(const std::string& _path)
    {
        m_screenWidth = 800;
        m_screenHeight = 600;
        m_resourcesRootPath = _path;

        std::cout << "root path : " << m_resourcesRootPath.c_str() << std::endl;

        m_fileSystem = new FileSystem();
        m_fileSystem->Init(m_resourcesRootPath);

        glfwInit();
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_SAMPLES, 4);
        //glfwWindowHint(GLFW_DECORATED, 0); // <- borderless window, nice to create a windowed full screen mode or lol like client

        m_window = glfwCreateWindow((int)m_screenWidth, (int)m_screenHeight, "Rain Engine", NULL, NULL);
        if (m_window == NULL)
        {
            std::cout << "Failed to create GLFW window" << std::endl;
            glfwTerminate();
            return -1;
        }

        //glfwSwapInterval(1); // Enable vsync
        //const GLFWvidmode* mode = glfwGetVideoMode(monitor);
        //glfwSetWindowMonitor(m_window, monitor, 0, 0, mode->width, mode->height, mode->refreshRate);

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

        glEnable(GL_MULTISAMPLE);

        glViewport(0, 0, (GLsizei)m_screenWidth, (GLsizei)m_screenHeight);
        glfwSetFramebufferSizeCallback(m_window, GameEngine::framebuffer_size_callback);

        glfwSetInputMode(m_window, GLFW_STICKY_KEYS, 1);

        m_inputEngine = new InputEngine(m_window);
        m_cameraController = new CameraController();
        m_cameraController->Init(m_inputEngine);

        initUI();
        return 0;
    }


    int GameEngine::Init(std::unordered_map<std::string, std::string> _args)
    {
        m_screenWidth = 800;
        m_screenHeight = 600;
        m_resourcesRootPath = Rain::GetExePath() + "/resources/";
        if (_args["root"] != "")
        {
            m_resourcesRootPath = _args["root"];
        }
        if (_args["s_width"] != "")
        {
            m_screenWidth = stof(_args["s_width"]);
        }
        if (_args["s_height"] != "")
        {
            m_screenHeight = stof(_args["s_height"]);
        }
        std::cout << "root path : " << m_resourcesRootPath.c_str() << std::endl;
	
		m_fileSystem = new FileSystem();
		m_fileSystem->Init(m_resourcesRootPath);

        glfwInit();
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_SAMPLES, 4);
        //glfwWindowHint(GLFW_DECORATED, 0); // <- borderless window, nice to create a windowed full screen mode or lol like client

        m_window = glfwCreateWindow((int)m_screenWidth, (int)m_screenHeight, "Rain Engine", NULL, NULL);
        if (m_window == NULL)
        {
            std::cout << "Failed to create GLFW window" << std::endl;
            glfwTerminate();
            return -1;
        }

		//glfwSwapInterval(1); // Enable vsync
        //const GLFWvidmode* mode = glfwGetVideoMode(monitor);
        //glfwSetWindowMonitor(m_window, monitor, 0, 0, mode->width, mode->height, mode->refreshRate);

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

        glEnable(GL_MULTISAMPLE);

        glViewport(0, 0, (GLsizei)m_screenWidth, (GLsizei)m_screenHeight);
        glfwSetFramebufferSizeCallback(m_window, GameEngine::framebuffer_size_callback);

        glfwSetInputMode(m_window, GLFW_STICKY_KEYS, 1);

        m_inputEngine = new InputEngine(m_window);
        m_cameraController = new CameraController();
        m_cameraController->Init(m_inputEngine);

		initUI();
        return 0;
    }

    void GameEngine::SetUpdateCallback(std::function<void(void)> _function)
    {
        m_updateCallback = _function;
    }

    void GameEngine::Run()
    {
		show_demo_window = true;
		show_another_window = false;
		clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

        while (!glfwWindowShouldClose(m_window))
        {
			updateUI();
			

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
			if (Rain::Input()->IsKeyPressed(GLFW_KEY_LEFT_CONTROL))
			{
				glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
				m_cameraController->Tick();
				GetCamera()->Tick();
			}
			else
			{
				glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
			}

            glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
			glEnable(GL_STENCIL_TEST);
			glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
            // rendering
            m_updateCallback();

			glDisable(GL_STENCIL_TEST);

			ImGui::Render();
			ImGui_ImplGlfwGL3_RenderDrawData(ImGui::GetDrawData());
            glfwSwapBuffers(m_window);
            glfwPollEvents();
        }

        glfwTerminate();
    }

	void GameEngine::updateUI()
	{
		ImGui_ImplGlfwGL3_NewFrame();

		// 1. Show a simple window.
		// Tip: if we don't call ImGui::Begin()/ImGui::End() the widgets automatically appears in a window called "Debug".
		{
			ImGui::SetNextWindowSize(ImVec2(400, 500), ImGuiCond_Always);
			ImGui::Begin("Debug");
			ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
			
			if (ImGui::TreeNode("Resources : "))
			{
				m_fileSystem->PrintToUI();
				

				//for (int i = 0; i < 5; i++)
				//	if (ImGui::TreeNode((void*)(intptr_t)i, "Child %d", i))
				//	{
				//		ImGui::Text("blah blah");
				//		ImGui::SameLine();
				//		if (ImGui::SmallButton("button")) {};
				//		ImGui::TreePop();
				//	}
				ImGui::TreePop();
			}

			ImGui::End();
		}

		// 2. Show another simple window. In most cases you will use an explicit Begin/End pair to name your windows.
		if (show_another_window)
		{
			ImGui::Begin("Another Window", &show_another_window);
			ImGui::Text("Hello from another window!");
			if (ImGui::Button("Close Me"))
				show_another_window = false;
			ImGui::End();
		}

		// 3. Show the ImGui demo window. Most of the sample code is in ImGui::ShowDemoWindow(). Read its code to learn more about Dear ImGui!
		if (show_demo_window)
		{
			ImGui::SetNextWindowPos(ImVec2(650, 20), ImGuiCond_FirstUseEver); // Normally user code doesn't need/want to call this because positions are saved in .ini file anyway. Here we just want to make the demo initial state a bit more friendly!
			ImGui::ShowDemoWindow(&show_demo_window);
		}
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

    void GameEngine::SetResourcesRoot(const std::string& _path)
    {
        m_resourcesRootPath = _path;
    }

    float GameEngine::GetDeltaTime()
    {
        return m_deltaTime;
    }

    glm::vec2 GameEngine::GetWindowSize()
    {
        return glm::vec2(m_screenWidth, m_screenHeight);
    }

    void GameEngine::framebuffer_size_callback(::GLFWwindow* window, int width, int height)
    {
        glViewport(0, 0, width, height);

    }

	void GameEngine::initUI()
	{

		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		//io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;  // Enable Keyboard Controls
		//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;   // Enable Gamepad Controls
		ImGui_ImplGlfwGL3_Init(m_window, true);

		// Setup style
		ImGui::StyleColorsDark();
		//ImGui::StyleColorsClassic();

		// Load Fonts
		// - If no fonts are loaded, dear imgui will use the default font. You can also load multiple fonts and use ImGui::PushFont()/PopFont() to select them. 
		// - AddFontFromFileTTF() will return the ImFont* so you can store it if you need to select the font among multiple. 
		// - If the file cannot be loaded, the function will return NULL. Please handle those errors in your application (e.g. use an assertion, or display an error and quit).
		// - The fonts will be rasterized at a given size (w/ oversampling) and stored into a texture when calling ImFontAtlas::Build()/GetTexDataAsXXXX(), which ImGui_ImplXXXX_NewFrame below will call.
		// - Read 'misc/fonts/README.txt' for more instructions and details.
		// - Remember that in C/C++ if you want to include a backslash \ in a string literal you need to write a double backslash \\ !
		//io.Fonts->AddFontDefault();
		//io.Fonts->AddFontFromFileTTF("../../misc/fonts/Roboto-Medium.ttf", 16.0f);
		//io.Fonts->AddFontFromFileTTF("../../misc/fonts/Cousine-Regular.ttf", 15.0f);
		//io.Fonts->AddFontFromFileTTF("../../misc/fonts/DroidSans.ttf", 16.0f);
		//io.Fonts->AddFontFromFileTTF("../../misc/fonts/ProggyTiny.ttf", 10.0f);
		//ImFont* font = io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\ArialUni.ttf", 18.0f, NULL, io.Fonts->GetGlyphRangesJapanese());
		//IM_ASSERT(font != NULL);

	}
}

