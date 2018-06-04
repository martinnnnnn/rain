#include "camera.h"


#include "rain.h"
#include "input/input.h"
#include "core/game_engine.h"

namespace rain
{
    //int Camera::Init(Game* _game)
    //{
    //    game = _game;
    //    m_movementSpeed = 200;
    //    m_worldUp = glm::vec3(0.0, 1.0, 0.0);
    //    Front = glm::vec3(0.0, 0.0, -1.0f);
    //    m_yaw = 0.0;
    //    m_pitch = 0.0f;
    //    UpdateRotation();
    //    updateProjectionMatrix(glm::vec2(game->gfxContext.width, game->gfxContext.height));
    //    return 0;
    //}

    //void Camera::Tick()
    //{
    //    UpdatePosition();
    //    UpdateRotation();
    //}

    //void Camera::UpdatePosition()
    //{
    //    float velocity = m_movementSpeed * game->deltaTime;
    //    glm::vec3 movement(0.0f, 0.0f, 0.0f);
    //    glm::vec3 front = Front;
    //    glm::vec3 right = Right;

    //    if (IsKeyPressed(game->input, GLFW_KEY_W))
    //    {
    //        movement += front * velocity;
    //    }
    //    if (IsKeyPressed(game->input, GLFW_KEY_S))
    //    {
    //        movement -= front * velocity;
    //    }
    //    if (IsKeyPressed(game->input, GLFW_KEY_A))
    //    {
    //        movement -= right * velocity;
    //    }
    //    if (IsKeyPressed(game->input, GLFW_KEY_D))
    //    {
    //        movement += right * velocity;
    //    }
    //    Translate(transform, movement);
    //}

    //void Camera::UpdateRotation()
    //{
    //    glm::vec2 offset = game->input->mouseOffset;

    //    offset.x *= 0.1f;
    //    offset.y *= 0.1f;

    //    m_yaw += offset.x;
    //    m_pitch += offset.y;

    //    if (m_pitch > 89.0f)
    //        m_pitch = 89.0f;
    //    if (m_pitch < -89.0f)
    //        m_pitch = -89.0f;

    //    SetRotation(transform, glm::vec3(0, -1, 0), m_yaw);
    //    Rotate(transform, glm::vec3(-1, 0, 0), m_pitch);

    //    glm::mat4 rotMat = glm::mat4_cast(transform.rotation);

    //    Front = glm::normalize(rotMat[2]);

    //    Right = glm::normalize(glm::cross(Front, m_worldUp));
    //    m_up = glm::normalize(glm::cross(Right, Front));
    //}


    //glm::mat4 Camera::GetViewMatrix(const glm::vec3& m_position)
    //{
    //    return glm::lookAt(m_position, m_position + Front, m_up);
    //}

    //Transform* Camera::GetTransform()
    //{
    //    return &transform;
    //}
    //glm::mat4 Camera::GetProjectionMatrix()
    //{
    //    return m_projectionMatrix;
    //}

    //void Camera::updateProjectionMatrix(const glm::vec2& _screen)
    //{
    //    m_projectionMatrix = glm::perspective(glm::radians(45.0f), (float)_screen.x / (float)_screen.y, 0.1f, 10000.0f);
    //}

    CameraS* GetDefaultCamera(Game* _game)
    {
        CameraS* camera = new CameraS();
        camera->game = _game;
        camera->movementSpeed = 100;
        camera->worldUp = glm::vec3(0.0, 1.0, 0.0);;
        camera->front = glm::vec3(0.0, 0.0, -1.0f);
        camera->yaw = 0.0f;
        camera->pitch = 0.0f;
        UpdateCamera(*camera);
        UpdateProjectionMatrix(*camera, glm::vec2(camera->game->gfxContext.width, camera->game->gfxContext.height));
        return camera;
    }


    void UpdateCamera(CameraS& _camera)
    {
        float velocity = _camera.movementSpeed * _camera.game->deltaTime;
        glm::vec3 movement(0.0f, 0.0f, 0.0f);
        glm::vec3 front = _camera.front;
        glm::vec3 right = _camera.right;
        Input* input = _camera.game->input;

        if (IsKeyPressed(input, GLFW_KEY_W))
        {
            movement += front * velocity;
        }
        if (IsKeyPressed(input, GLFW_KEY_S))
        {
            movement -= front * velocity;
        }
        if (IsKeyPressed(input, GLFW_KEY_A))
        {
            movement -= right * velocity;
        }
        if (IsKeyPressed(input, GLFW_KEY_D))
        {
            movement += right * velocity;
        }
        Translate(_camera.transform, movement);

        glm::vec2 offset = input->mouseOffset;

        offset.x *= 0.1f;
        offset.y *= 0.1f;

        _camera.yaw += offset.x;
        _camera.pitch += offset.y;

        if (_camera.pitch > 89.0f)
            _camera.pitch = 89.0f;
        if (_camera.pitch < -89.0f)
            _camera.pitch = -89.0f;

        SetRotation(_camera.transform, glm::vec3(0, -1, 0), _camera.yaw);
        Rotate(_camera.transform, glm::vec3(-1, 0, 0), _camera.pitch);

        glm::mat4 rotMat = glm::mat4_cast(_camera.transform.rotation);

        _camera.front = glm::normalize(rotMat[2]);

        _camera.right = glm::normalize(glm::cross(_camera.front, _camera.worldUp));
        _camera.up = glm::normalize(glm::cross(_camera.right, _camera.front));
    }

    glm::mat4 GetViewMatrix(CameraS& _camera)
    {
        return glm::lookAt(_camera.transform.position, _camera.transform.position + _camera.front, _camera.up);
    }

    glm::mat4 GetProjectionMatrix(CameraS& _camera)
    {
        return _camera.projectionMatrix;
    }

    void UpdateProjectionMatrix(CameraS& _camera, const glm::vec2& _screen)
    {
        UpdateProjectionMatrix(_camera, (float)_screen.x, (float)_screen.y);
    }

    void UpdateProjectionMatrix(CameraS& _camera, float width, float height)
    {
        _camera.projectionMatrix = glm::perspective(glm::radians(45.0f), width / height, 0.1f, 1000.0f);
    }
}