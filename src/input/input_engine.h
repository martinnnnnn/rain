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


    struct Input
    {
        GLFWwindow* window;
        glm::vec2 mouseLastPos;
        glm::vec2 mouseOffset;
    };

	//glm::vec2 GetMouseOffset();
	glm::vec2 GetMousePosition(Input* _input);
	bool IsKeyPressed(Input* _input, int _keyCode);
    void ComputeMouseOffset(Input* _input);

}