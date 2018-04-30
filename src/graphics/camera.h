#pragma once


#include "utility/stdincl_3d.h"


namespace rain
{
    class Camera
    {
    public:
        Camera();

        void Tick();
        glm::mat4 GetViewMatrix(const glm::vec3& _position);
        glm::mat4 GetProjectionMatrix();
        glm::vec3 Front();
        glm::vec3 Right();
    private:
        void updateCameraVectors();
        void processMouseMouvement(const glm::vec2& _mouseOffset);
        void updateProjectionMatrix(const glm::vec2& _screen);

        glm::mat4 m_projectionMatrix;
	    glm::vec3 m_front;
	    glm::vec3 m_up;
	    glm::vec3 m_right;
	    glm::vec3 m_worldUp;
        float m_yaw;
        float m_pitch;

    };
}