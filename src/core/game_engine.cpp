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
#include "input/input.h"
#include "file_system.h"
#include "utility/string_utils.h"

namespace rain
{
    //void GameEngine::Run()
    //{
		//show_demo_window = true;
		//show_another_window = false;
		//clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

  //      while (!glfwWindowShouldClose(m_window))
  //      {
		//	updateUI();
		//	

  //          // inputs relative to window
  //          if (m_inputEngine->IsKeyPressed(GLFW_KEY_ESCAPE))
  //          {
  //              glfwSetWindowShouldClose(m_window, true);
  //          }

  //          // delta time update
  //          float currentFrame = (float)glfwGetTime();
  //          m_deltaTime = currentFrame - m_lastFrame;
  //          m_lastFrame = currentFrame;

  //          // input
  //          m_inputEngine->Tick();

  //          // objects
		//	if (Rain::Input()->IsKeyPressed(GLFW_KEY_LEFT_CONTROL))
		//	{
		//		glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		//		m_cameraController->Tick();
		//		GetCamera()->Tick();
		//	}
		//	else
		//	{
		//		glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		//	}

  //          glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
		//	glEnable(GL_STENCIL_TEST);
		//	glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
  //          glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
  //          // rendering
  //          m_updateCallback();

		//	glDisable(GL_STENCIL_TEST);

		//	ImGui::Render();
		//	ImGui_ImplGlfwGL3_RenderDrawData(ImGui::GetDrawData());
  //          glfwSwapBuffers(m_window);
  //          glfwPollEvents();
  //      }

  //      glfwTerminate();
 //   }

	//void GameEngine::updateUI()
	//{
	//	ImGui_ImplGlfwGL3_NewFrame();

	//	// 1. Show a simple window.
	//	// Tip: if we don't call ImGui::Begin()/ImGui::End() the widgets automatically appears in a window called "Debug".
	//	{
	//		ImGui::SetNextWindowSize(ImVec2(400, 500), ImGuiCond_Always);
	//		ImGui::Begin("Debug");
	//		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	//		
	//		if (ImGui::TreeNode("Resources : "))
	//		{
	//			m_fileSystem->PrintToUI();
	//			

	//			//for (int i = 0; i < 5; i++)
	//			//	if (ImGui::TreeNode((void*)(intptr_t)i, "Child %d", i))
	//			//	{
	//			//		ImGui::Text("blah blah");
	//			//		ImGui::SameLine();
	//			//		if (ImGui::SmallButton("button")) {};
	//			//		ImGui::TreePop();
	//			//	}
	//			ImGui::TreePop();
	//		}

	//		ImGui::End();
	//	}

	//	// 2. Show another simple window. In most cases you will use an explicit Begin/End pair to name your windows.
	//	if (show_another_window)
	//	{
	//		ImGui::Begin("Another Window", &show_another_window);
	//		ImGui::Text("Hello from another window!");
	//		if (ImGui::Button("Close Me"))
	//			show_another_window = false;
	//		ImGui::End();
	//	}

	//	// 3. Show the ImGui demo window. Most of the sample code is in ImGui::ShowDemoWindow(). Read its code to learn more about Dear ImGui!
	//	if (show_demo_window)
	//	{
	//		ImGui::SetNextWindowPos(ImVec2(650, 20), ImGuiCond_FirstUseEver); // Normally user code doesn't need/want to call this because positions are saved in .ini file anyway. Here we just want to make the demo initial state a bit more friendly!
	//		ImGui::ShowDemoWindow(&show_demo_window);
	//	}
	//}


	//void GameEngine::initUI()
	//{

	//	IMGUI_CHECKVERSION();
	//	ImGui::CreateContext();
	//	ImGuiIO& io = ImGui::GetIO(); (void)io;
	//	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;  // Enable Keyboard Controls
	//	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;   // Enable Gamepad Controls
	//	ImGui_ImplGlfwGL3_Init(m_window, true);

	//	// Setup style
	//	ImGui::StyleColorsDark();
	//	//ImGui::StyleColorsClassic();

	//	// Load Fonts
	//	// - If no fonts are loaded, dear imgui will use the default font. You can also load multiple fonts and use ImGui::PushFont()/PopFont() to select them. 
	//	// - AddFontFromFileTTF() will return the ImFont* so you can store it if you need to select the font among multiple. 
	//	// - If the file cannot be loaded, the function will return NULL. Please handle those errors in your application (e.g. use an assertion, or display an error and quit).
	//	// - The fonts will be rasterized at a given size (w/ oversampling) and stored into a texture when calling ImFontAtlas::Build()/GetTexDataAsXXXX(), which ImGui_ImplXXXX_NewFrame below will call.
	//	// - Read 'misc/fonts/README.txt' for more instructions and details.
	//	// - Remember that in C/C++ if you want to include a backslash \ in a string literal you need to write a double backslash \\ !
	//	//io.Fonts->AddFontDefault();
	//	//io.Fonts->AddFontFromFileTTF("../../misc/fonts/Roboto-Medium.ttf", 16.0f);
	//	//io.Fonts->AddFontFromFileTTF("../../misc/fonts/Cousine-Regular.ttf", 15.0f);
	//	//io.Fonts->AddFontFromFileTTF("../../misc/fonts/DroidSans.ttf", 16.0f);
	//	//io.Fonts->AddFontFromFileTTF("../../misc/fonts/ProggyTiny.ttf", 10.0f);
	//	//ImFont* font = io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\ArialUni.ttf", 18.0f, NULL, io.Fonts->GetGlyphRangesJapanese());
	//	//IM_ASSERT(font != NULL);

	//}



}

