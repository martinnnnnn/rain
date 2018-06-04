#include "input.h"

#include "utility/first_time_mcr.h"
#include "utility/incl_3d.h"

namespace rain
{
	glm::vec2 GetMousePosition(Input* _input)
	{
		double xpos, ypos;
		glfwGetCursorPos(_input->window, &xpos, &ypos);
		return glm::vec2(xpos, ypos);
	}

	bool IsKeyPressed(Input* _input, int _keyCode)
	{
		if (glfwGetKey(_input->window, _keyCode) == GLFW_PRESS)
		{
			return true;
		}
		return false;
	}

    void ComputeMouseOffset(Input* _input)
    {
        double xpos, ypos;
        glfwGetCursorPos(_input->window, &xpos, &ypos);

        if (FIRST_TIME_HERE)
        {
            
            _input->mouseLastPos.x = (float)xpos;
            _input->mouseLastPos.y = (float)ypos;
        }

        _input->mouseOffset.x = (float)xpos - _input->mouseLastPos.x;
        _input->mouseOffset.y = _input->mouseLastPos.y - (float)ypos;
        _input->mouseLastPos.x = (float)xpos;
        _input->mouseLastPos.y = (float)ypos;
    }
}