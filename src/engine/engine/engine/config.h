#pragma once

#include <string>

#include "core/core.h"

using namespace rain::core;

namespace rain::engine
{
	struct Config
	{
		void init(const std::string& _path);

		File file;
		std::string engine_name;
		std::string data_root;
		std::string starting_world;
		u32 screen_width;
		u32 screen_height;
		bool full_screen;
	};
}
