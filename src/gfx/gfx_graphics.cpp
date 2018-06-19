#include "gfx/gfx_graphics.h"


#include <iostream>

#include "utility/incl_3d.h"



namespace rain
{
	GFXContext InitWindow(const std::string& name, int _width, int _height)
	{
		GFXContext context = {};
		context.width = _width;
		context.height = _height;

		glfwInit();
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		glfwWindowHint(GLFW_SAMPLES, 4);
        glfwWindowHint(GLFW_DOUBLEBUFFER, true);
		glfwWindowHint(GLFW_DECORATED, 0); // <- borderless window, nice to create a windowed full screen mode or lol like client

		context.window = glfwCreateWindow(context.width, context.height, "Rain Engine", /*glfwGetPrimaryMonitor() <- to go full screen */NULL, NULL);
		if (context.window == NULL)
		{
			std::cout << "Failed to create GLFW window" << std::endl;
			glfwTerminate();
			goto terminate;
		}

		glfwMakeContextCurrent(context.window);

		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		{
			std::cout << "Failed to initialize GLAD" << std::endl;
			goto terminate;
		}

		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LESS);
		//TODO(martin): test these functions
		//glEnable(GL_CULL_FACE);
		//glFrontFace(GL_CW);

		//glEnable(GL_MULTISAMPLE);
        glfwSwapInterval(1);
		glViewport(0, 0, (GLsizei)context.width, (GLsizei)context.height);
		glfwSetFramebufferSizeCallback(context.window, framebuffer_size_callback);

        return context;
	terminate:
		context = {};
		return context;
	}

	void framebuffer_size_callback(GLFWwindow* window, int width, int height)
	{
		glViewport(0, 0, width, height);
	}
}



