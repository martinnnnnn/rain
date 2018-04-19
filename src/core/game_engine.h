#pragma once

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

class InputManager
{
public:
    InputManager(GLFWwindow* window);
    void Tick();


    glm::vec2 GetMouseOffset();
    glm::vec2 GetMousePosition();
    bool IsKeyPressed(int keyCode);

private:
    GLFWwindow* _window;
    glm::vec2 _lastPos;
    glm::vec2 _offset;
    bool _firstMouse;


};


class GameEngine
{
public:

    GameEngine();
    void Init();

    GLFWwindow* Getwindow();
    InputManager* GetInputManager();

private:
    InputManager* m_inputManager;
    GLFWwindow* m_window;
    float m_deltaTime;
    float m_screenWidth;
    float m_screenHeight;

};
