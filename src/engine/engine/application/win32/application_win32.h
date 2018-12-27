#pragma once

#include <string>
#include <unordered_map>
#include <vector>
#include <functional>

#include "engine/win32/helper_win32.h"
#include "core/core.h"

using namespace rain::core;

namespace rain::engine
{
	struct IdGenerator;
	struct World;
	struct Config;
	struct DataSystem;
	struct Client;

	class Window;
	class Renderer;
	class Input;
	class Logger;
	class HighResolutionClock;

	class Application : public Singleton<Application>
	{
	public:

		int init(HINSTANCE _hinstance, const std::string& _config);
		void shutdown();
		void update();
		void render(float _alpha);

		Config* config;
		Window* window;
		Renderer* renderer;
		Input* input;
		Logger* logger;
		IdGenerator* id_generator;
		DataSystem* data_system;
		Client* client;
	private:
		World* world;
		HINSTANCE hinstance;
		HighResolutionClock* m_clock;
	};
}
