#pragma once

#include "core/transform.h"

namespace rain
{
    class Camera;
    //class Transform;
    class InputEngine;
    struct Game;

    class CameraController
    {
    public:
        enum class Direction
        {
            FORWARD = 0,
            BACKWARD = 1,
            LEFT = 2,
            RIGHT = 3
        };

        CameraController() : 
            m_camera(nullptr)
        {}

        int Init(Game* _game);
        void Tick();
        Camera* GetCamera();
        TransformS* GetTransform();

    private:
        Camera* m_camera;
        TransformS transform;
        float m_movementSpeed;
        Game* game;
    };
}