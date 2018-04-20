                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                               #include "camera_controller.h"

#include <GLFW/glfw3.h>

#include "rain.h"
#include "core/game_engine.h"
#include "graphics/camera.h"
#include "core/transform.h"


int CameraController::Init(InputEngine* _inputEngine)
{
    m_inputEngine = _inputEngine;
    m_camera = new Camera();
    m_transform = new Transform();
    m_movementSpeed = 5;
    return 0;
}



void CameraController::Tick()
{
    float deltaTime = Rain::GetDeltaTime();
    float velocity = m_movementSpeed * deltaTime;
    glm::vec3 movement(0.0f, 0.0f, 0.0f);
    glm::vec3 front = m_camera->Front();
    glm::vec3 right = m_camera->Right();

    if (m_inputEngine->IsKeyPressed(GLFW_KEY_W))
    {
        movement += front * velocity;
    }
    if (m_inputEngine->IsKeyPressed(GLFW_KEY_S))
    {
        movement -= front * velocity;
    }
    if (m_inputEngine->IsKeyPressed(GLFW_KEY_A))
    {
        movement -= right * velocity;
    }
    if (m_inputEngine->IsKeyPressed(GLFW_KEY_D))
    {
        movement += right * velocity;
    }
    m_transform->Translate(movement);
}
                             

Camera* CameraController::GetCamera()
{
    return m_camera;
}

Transform* CameraController::GetTransform()
{
    return m_transform;
}