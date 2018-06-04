#include "rain.h"

#include <vector>
#include <string>
#include <windows.h>
#include <iterator>

#include "core/game_engine.h"
#include "utility/string_utils.h"

namespace rain
{
	//int Rain::Init(std::unordered_map<std::string, std::string> _args)
	//{
	//	return GameEngine::Get().Init(_args);
	//}

	//void Rain::Run()
	//{
	//	GameEngine::Get().Run();
	//}

	//GameEngine* Rain::Engine()
	//{
	//	return &(GameEngine::Get());
	//}

	//GLFWwindow* Rain::Window()
	//{
	//	return GameEngine::Get().Getwindow();
	//}

	//Camera* Rain::Camera()
	//{
	//	return GameEngine::Get().GetCamera();
	//}

	//InputEngine* Rain::Input()
	//{
	//	return GameEngine::Get().GetInputEngine();
	//}

	//float Rain::GetDeltaTime()
	//{
	//	return GameEngine::Get().GetDeltaTime();
	//}

	//std::string Rain::GetExePath()
	//{
	//	char buffer[MAX_PATH];
	//	GetModuleFileName(NULL, buffer, MAX_PATH);
	//	std::string::size_type pos = std::string(buffer).find_last_of("\\/");
	//	return std::string(buffer).substr(0, pos);
	//}

	//std::string Rain::ResourcesRoot()
	//{
	//	return GameEngine::Get().GetResourcesRoot();
	//}

	//glm::vec2 Rain::GetWindowSize()
	//{
	//	return Engine()->GetWindowSize();
	//}




}