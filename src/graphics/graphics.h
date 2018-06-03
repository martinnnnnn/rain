#pragma once


#include <stdint.h>

struct GLFWwindow;

namespace rain
{
	struct GFXContext
	{
		GLFWwindow* window;
		uint16_t width;
		uint16_t height;
	};


	GFXContext InitWindow(const std::string& name, int _width, int _height);


	void framebuffer_size_callback(GLFWwindow* window, int width, int height);
}