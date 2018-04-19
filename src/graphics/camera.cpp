#include "camera.h"

#include <glm/gtc/matrix_transform.hpp>


Camera::Camera()
{
    _worldUp = glm::vec3(0.0, 1.0, 0.0);
    _front = glm::vec3(0.0, 0.0, -1.0f);
    _yaw = -90.0;
    _pitch = 0.0f;
    
    updateCameraVectors();
}


void Camera::Tick(const glm::vec2& mouseOffset)
{
    processMouseMouvement(mouseOffset);
}

void Camera::processMouseMouvement(const glm::vec2& mouseOffset)
{
    glm::vec2 offset = mouseOffset;

    offset.x *= 0.1f;
    offset.y *= 0.1f;

    _yaw += offset.x;
    _pitch += offset.y;

    if (_pitch > 89.0f)
        _pitch = 89.0f;
    if (_pitch < -89.0f)
        _pitch = -89.0f;

    updateCameraVectors();
}

void Camera::updateCameraVectors()
{
    glm::vec3 front
    {
        cos(glm::radians(_yaw)) * cos(glm::radians(_pitch))
        ,sin(glm::radians(_pitch))
        ,sin(glm::radians(_yaw)) * cos(glm::radians(_pitch))
    };
    _front = glm::normalize(front);

    _right = glm::normalize(glm::cross(_front, _worldUp));
    _up = glm::normalize(glm::cross(_right, _front));
}

glm::mat4 Camera::GetViewMatrix(const glm::vec3& position)
{
    return glm::lookAt(position, position + _front, _up);
}

glm::mat4 Camera::GetProjectionMatrix()
{   
    return _projectionMatrix;
}

void Camera::updateProjectionMatrix(const glm::vec2& screen)
{
    _projectionMatrix = glm::perspective(glm::radians(45.0f), (float)screen.x / (float)screen.y, 0.1f, 100.0f);
}

