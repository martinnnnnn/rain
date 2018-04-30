#pragma once

#include "utility/stdincl_3d.h"


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
        bool m_firstMouse;
    };
}