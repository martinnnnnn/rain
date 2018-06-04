#pragma once

//#include "utility/incl_3d.h"

#include <glm/glm.hpp>

struct GLFWwindow;
namespace rain
{
    class InputEngine
    {
    public:
        InputEngine(GLFWwindow* window);
        void Tick();

        glm::vec2 GetMouseOffset();
        glm::vec2 GetMousePosition();
        bool IsKeyPressed(int keyCode);

    private:
        GLFWwindow * m_window;
        glm::vec2 m_lastPos;
        glm::vec2 m_offset;
    };

	//glm::vec2 GetMouseOffset();
	glm::vec2 GetMousePosition(GLFWwindow* _window);
	bool IsKeyPressed(GLFWwindow* _window, int _keyCode);

}