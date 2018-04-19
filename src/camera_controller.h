#pragma once


class Camera;
class Transform;
class InputManager;

class CameraController
{
    CameraController(Camera* camera, Transform* transform, InputManager* inputmanager) : 
        _camera(camera)
        , _transform(transform)
        , _inputManager(inputmanager)
    {}


    void Tick();

private:
    Camera* _camera;
    Transform* _transform;
    InputManager* _inputManager;
};