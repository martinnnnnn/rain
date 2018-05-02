#include "camera.h"


#include "rain.h"
#include "input/input_engine.h"


namespace rain
{
    Camera::Camera()
    {
        m_worldUp = glm::vec3(0.0, 1.0, 0.0);
        Front = glm::vec3(0.0, 0.0, -1.0f);
        m_yaw = -90.0;
        m_pitch = 0.0f;
    
        updateCameraVectors();
        updateProjectionMatrix(glm::vec2(800,600));
    }


    void Camera::Tick()
    {
    
        processMouseMouvement(Rain::Input()->GetMouseOffset());
    }

    void Camera::processMouseMouvement(const glm::vec2& _mouseOffset)
    {
        glm::vec2 offset = _mouseOffset;

        offset.x *= 0.1f;
        offset.y *= 0.1f;

        m_yaw += offset.x;
        m_pitch += offset.y;

        if (m_pitch > 89.0f)
            m_pitch = 89.0f;
        if (m_pitch < -89.0f)
            m_pitch = -89.0f;

        updateCameraVectors();
    }

    void Camera::updateCameraVectors()
    {
        glm::vec3 front
        {
            cos(glm::radians(m_yaw)) * cos(glm::radians(m_pitch))
            ,sin(glm::radians(m_pitch))
            ,sin(glm::radians(m_yaw)) * cos(glm::radians(m_pitch))
        };
        Front = glm::normalize(front);

        Right = glm::normalize(glm::cross(Front, m_worldUp));
        m_up = glm::normalize(glm::cross(Right, Front));
    }

    glm::mat4 Camera::GetViewMatrix(const glm::vec3& m_position)
    {
        return glm::lookAt(m_position, m_position + Front, m_up);
    }

    glm::mat4 Camera::GetProjectionMatrix()
    {   
        return m_projectionMatrix;
    }

    void Camera::updateProjectionMatrix(const glm::vec2& _screen)
    {
        m_projectionMatrix = glm::perspective(glm::radians(45.0f), (float)_screen.x / (float)_screen.y, 0.1f, 100.0f);
    }
}