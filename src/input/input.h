#pragma once

//#include "utility/incl_3d.h"

#include <glm/glm.hpp>

struct GLFWwindow;
namespace rain
{
    struct Input
    {
        GLFWwindow* window;
        glm::vec2 mouseLastPos;
        glm::vec2 mouseOffset;
    };

	glm::vec2 GetMousePosition(Input* _input);
	bool IsKeyPressed(Input* _input, int _keyCode);
    void ComputeMouseOffset(Input* _input);
}