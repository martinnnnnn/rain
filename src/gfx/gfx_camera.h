#pragma once


#include "utility/incl_3d.h"
#include "core/transform.h"
#include "ecs/ecs_component.h"

namespace rain
{
    struct Game;

    struct Camera : public Component
    {
        enum class Direction
        {
            FORWARD = 0,
            BACKWARD = 1,
            LEFT = 2,
            RIGHT = 3
        };

        glm::vec3 front;
        glm::vec3 right;
        float yaw;
        float pitch;
        Transform* transform;
        float movementSpeed;
        glm::vec3 worldUp;
        glm::vec3 up;
        Game* game;
        glm::mat4 projectionMatrix;
    };

    Camera* GetDefaultCamera(Game* _game);
    void UpdateCamera(Camera& _camera);
    glm::mat4 GetViewMatrix(Camera& _camera);
    void UpdateProjectionMatrix(Camera& _camera, const glm::vec2& _screen);
    void UpdateProjectionMatrix(Camera& _camera, float width, float height);
}