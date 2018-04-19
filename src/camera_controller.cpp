#include "camera_controller.h"

#include "core/game_engine.h"


void CameraController::Tick()
{
    float deltaTime = GetDeltaTime();
    float velocity = _movementSpeed * deltaTime;
    glm::vec3 movement(0.0f, 0.0f, 0.0f);
    glm::vec3 front = _camera->Front;
    glm::vec3 right = _camera->Right;

    if (_inputManager->IsKeyPressed(GLFW_KEY_W))
    {
        _transform->Translate()
    }
    if (_inputManager->IsKeyPressed(GLFW_KEY_W))
    {
        ProcessMovement(BACKWARD);
    }
    if (_inputManager->IsKeyPressed(GLFW_KEY_W))
    {
        ProcessMovement(LEFT);
    }
    if (_inputManager->IsKeyPressed(GLFW_KEY_W))
    {
        ProcessMovement(RIGHT);
    }
}

