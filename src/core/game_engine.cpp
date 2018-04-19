#include "game_engine.h"




InputManager::InputManager(GLFWwindow* window) :
    _window(window)
{
    _window.
}

void InputManager::Tick()
{
    double xpos, ypos;
    glfwGetCursorPos(_window, &xpos, &ypos);

    if (_firstMouse)
    {
        _lastPos.x = xpos;
        _lastPos.y = ypos;
        _firstMouse = false;
    }
    _offset.x = xpos - _lastPos.x;
    _offset.y = _lastPos.y - ypos;
}

bool InputManager::IsKeyPressed(int keyCode)
{
    if (glfwGetKey(_window, keyCode == GLFW_PRESS))
    {
        return true;
    }
    return false;
}


glm::vec2 InputManager::GetMousePosition()
{
    double xpos, ypos;
    glfwGetCursorPos(_window, &xpos, &ypos);
    return glm::vec2(xpos, ypos);
}

glm::vec2 InputManager::GetMouseOffset()
{
    return _offset;
}

//----------------------------------------------------------
GameEngine::GameEngine() :
    m_inputManager(nullptr),
    m_window(nullptr)
{

}

void GameEngine::Init()
{
    m_window
    m_inputManager = new InputManager();
}

GLFWwindow* GameEngine::Getwindow()
{

}

InputManager* GameEngine::GetInputManager()
{

}