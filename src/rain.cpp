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
#include "camera_controller.h"
#include "graphics/camera.h"
#include "input/input.h"
#include "core/file_system.h"
#include "utility/string_utils.h"

namespace rain
{

    Game InitGame(const std::string& _dataPath)
    {
        Game game = {};
        game.dataPath = _dataPath;
        game.gfxContext = InitWindow("Rain Engine", 1920, 1080);

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
        //game->inputEngine = new InputEngine(game->gfxContext.window);
        game->cameraController = new CameraController();
        game->cameraController->Init(game);

        return game;
    }

    void RunGame(Game* _game, std::function<void(void)> _updateCallBack)
    {
        while (!glfwWindowShouldClose(_game->gfxContext.window))
        {
            // inputs relative to window
            if (IsKeyPressed(_game->input, GLFW_KEY_ESCAPE))
                //if (_game->inputEngine->IsKeyPressed(GLFW_KEY_ESCAPE))
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
                _game->cameraController->Tick();
                _game->cameraController->GetCamera()->Tick();
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

            //ImGui::Render();
            //ImGui_ImplGlfwGL3_RenderDrawData(ImGui::GetDrawData());
            glfwSwapBuffers(_game->gfxContext.window);
            glfwPollEvents();
        }

        glfwTerminate();
    }
}