#include "input_engine.h"

#include "utility/first_time_mcr.h"
#include "utility/incl_3d.h"

namespace rain
{
	InputEngine::InputEngine(GLFWwindow* _window) :
        m_window(_window)
    {
    }

    void InputEngine::Tick()
    {
        double xpos, ypos;
        glfwGetCursorPos(m_window, &xpos, &ypos);

        if (FIRST_TIME_HERE)
        {
            m_lastPos.x = (float)xpos;
            m_lastPos.y = (float)ypos;
        }

        m_offset.x = (float)xpos - m_lastPos.x;
        m_offset.y = m_lastPos.y - (float)ypos;
        m_lastPos.x = (float)xpos;
        m_lastPos.y = (float)ypos;
    }

    bool InputEngine::IsKeyPressed(int _keyCode)
    {
        if (glfwGetKey(m_window, _keyCode) == GLFW_PRESS)
        {
            return true;
        }
        return false;
    }


    glm::vec2 InputEngine::GetMousePosition()
    {
        double xpos, ypos;
        glfwGetCursorPos(m_window, &xpos, &ypos);
        return glm::vec2(xpos, ypos);
    }

    glm::vec2 InputEngine::GetMouseOffset()
    {
        return m_offset;
    }

	//glm::vec2 GetMouseOffset()
	//{

	//}

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