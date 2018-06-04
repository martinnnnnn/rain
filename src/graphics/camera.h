#pragma once


#include "utility/incl_3d.h"
#include "core/transform.h"

namespace rain
{
    struct Game;

    class Camera
    {
    public:
        enum class Direction
        {
            FORWARD = 0,
            BACKWARD = 1,
            LEFT = 2,
            RIGHT = 3
        };


        int Init(Game* _game);
        void Tick();
        void UpdatePosition();
        void UpdateRotation();

        Transform* GetTransform();
        glm::mat4 GetViewMatrix(const glm::vec3& m_position);
        glm::mat4 GetProjectionMatrix();
        void updateProjectionMatrix(const glm::vec2& _screen);
        glm::vec3 Front;
        glm::vec3 Right;
    private:
        float m_yaw;
        float m_pitch;
        Transform transform;
        float m_movementSpeed;
        glm::vec3 m_worldUp;
        glm::vec3 m_up;
        Game* game;
        glm::mat4 m_projectionMatrix;
    };
}