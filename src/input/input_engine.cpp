#include "input_engine.h"


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

        static bool once = [this](double xpos, double ypos)
        {
            m_lastPos.x = (float)xpos;
            m_lastPos.y = (float)ypos;
            return true;
        } (xpos, ypos);

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
}