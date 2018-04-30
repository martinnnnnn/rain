#pragma once


namespace rain
{
    class Camera;
    class Transform;
    class InputEngine;

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
            , m_transform(nullptr)
            , m_inputEngine(nullptr)
        {}

        int Init(InputEngine* _inputEngine);
        void Tick();
        Camera* GetCamera();
        Transform* GetTransform();

    private:
        Camera* m_camera;
        Transform* m_transform;
        InputEngine* m_inputEngine;
        float m_movementSpeed;
    };
}