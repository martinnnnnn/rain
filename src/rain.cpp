#include "rain.h"

#include <iostream>
#include <vector>
#include <string>
#include <windows.h>
#include <iterator>

#include <filesystem>
using namespace std::experimental::filesystem::v1;

#include "imgui.h"
#include "imgui_impl_glfw_gl3.h"

#include "utility/incl_3d.h"
#include "graphics/camera.h"
#include "input/input.h"
#include "core/file_system.h"
#include "utility/string_utils.h"

namespace rain
{

    Game* InitGame(const std::string& _dataPath)
    {
        Game* game = new Game();
        game->dataPath = _dataPath;
        game->gfxContext = InitWindow("Rain Engine", 800, 600);
        game->input = new Input();
        game->input->window = game->gfxContext.window;
        game->camera = GetDefaultCamera(game);
        //game->camera->Init(game);
        InitUI(game->gfxContext.window);
        return game;
    }

    Game* InitGame(int argc, char** argv)
    {
        Game* game = new Game();
        auto arguments = String::GetArguments(argc, argv);
        if (arguments["root"] != "")
        {
            game->dataPath = arguments["root"];
        }
        game->gfxContext = InitWindow("Rain Engine", 800, 600);
        game->input = new Input();
        game->input->window = game->gfxContext.window;
        game->camera = GetDefaultCamera(game);
        //game->camera->Init(game);
        InitUI(game->gfxContext.window);
        return game;
    }

    void RunGame(Game* _game, std::function<void(void)> _updateCallBack)
    {
        while (!glfwWindowShouldClose(_game->gfxContext.window))
        {
            UpdateUI();

            // inputs relative to window
            if (IsKeyPressed(_game->input, GLFW_KEY_ESCAPE))
            {
                glfwSetWindowShouldClose(_game->gfxContext.window, true);
            }

            // delta time update
            float currentFrame = (float)glfwGetTime();
            _game->deltaTime = currentFrame - _game->lastFrame;
            _game->lastFrame = currentFrame;

            // input
            ComputeMouseOffset(_game->input);

            // objects
            if (IsKeyPressed(_game->input, GLFW_KEY_LEFT_CONTROL))
            {
                glfwSetInputMode(_game->gfxContext.window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
                UpdateCamera(*_game->camera);
                //_game->camera->Tick();
            }
            else
            {
                glfwSetInputMode(_game->gfxContext.window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
            }

            glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
            glEnable(GL_STENCIL_TEST);
            glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
            // rendering
            _updateCallBack();

            glDisable(GL_STENCIL_TEST);

            ImGui::Render();
            ImGui_ImplGlfwGL3_RenderDrawData(ImGui::GetDrawData());

            glfwSwapBuffers(_game->gfxContext.window);
            glfwPollEvents();
        }

        glfwTerminate();
    }

    void UpdateUI()
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
                //m_fileSystem->PrintToUI();
                
                    
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
        
        //// 2. Show another simple window. In most cases you will use an explicit Begin/End pair to name your windows.
        //if (show_another_window)
        //{
        //    ImGui::Begin("Another Window", &show_another_window);
        //    ImGui::Text("Hello from another window!");
        //    if (ImGui::Button("Close Me"))
        //        show_another_window = false;
        //    ImGui::End();
        //}
        //
        //// 3. Show the ImGui demo window. Most of the sample code is in ImGui::ShowDemoWindow(). Read its code to learn more about Dear ImGui!
        //if (show_demo_window)
        //{
        //ImGui::SetNextWindowPos(ImVec2(650, 20), ImGuiCond_FirstUseEver); // Normally user code doesn't need/want to call this because positions are saved in .ini file anyway. Here we just want to make the demo initial state a bit more friendly!
        //ImGui::ShowDemoWindow(&show_demo_window);
        //}
    }

    void InitUI(GLFWwindow* _window)
    {
        
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO & io = ImGui::GetIO(); (void)io;
        //io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;  // Enable Keyboard Controls
        //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;   // Enable Gamepad Controls
        ImGui_ImplGlfwGL3_Init(_window, true);
        
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