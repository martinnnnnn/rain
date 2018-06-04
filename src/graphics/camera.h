#pragma once


#include "utility/incl_3d.h"


namespace rain
{
    struct Game;
    class Camera
    {
    public:
        Camera(Game* _game);

        void Tick();
        glm::mat4 GetViewMatrix(const glm::vec3& _position);
        glm::mat4 GetProjectionMatrix();
	    glm::vec3 Front;
	    glm::vec3 Right;

    private:
        void updateCameraVectors();
        void processMouseMouvement(const glm::vec2& _mouseOffset);
        void updateProjectionMatrix(const glm::vec2& _screen);

        glm::mat4 m_projectionMatrix;
	    glm::vec3 m_worldUp;
	    glm::vec3 m_up;
        float m_yaw;
        float m_pitch;
        Game* game;

    };
}