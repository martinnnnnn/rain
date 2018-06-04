#include "camera_controller.h"


#include "rain.h"
#include "input/input.h"
#include "graphics/camera.h"
#include "core/transform.h"
#include "core/game_engine.h"

namespace rain
{
    int CameraController::Init(Game* _game)
    {
        game = _game;
        m_camera = new Camera(game);
        m_movementSpeed = 200;
        return 0;
    }

    void CameraController::Tick()
    {
        float velocity = m_movementSpeed * game->deltaTime;
        glm::vec3 movement(0.0f, 0.0f, 0.0f);
        glm::vec3 front = m_camera->Front;
        glm::vec3 right = m_camera->Right;

        if (IsKeyPressed(game->input, GLFW_KEY_W))
        {
            movement += front * velocity;
        }
        if (IsKeyPressed(game->input, GLFW_KEY_S))
        {
            movement -= front * velocity;
        }
        if (IsKeyPressed(game->input, GLFW_KEY_A))
        {
            movement -= right * velocity;
        }
        if (IsKeyPressed(game->input, GLFW_KEY_D))
        {
            movement += right * velocity;
        }
        Translate(transform, movement);
    }
                             

    Camera* CameraController::GetCamera()
    {
        return m_camera;
    }

    TransformS* CameraController::GetTransform()
    {
        return &transform;
    }
}