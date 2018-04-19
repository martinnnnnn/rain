#pragma once

#include <glm/glm.hpp>

class Camera
{
public:
    Camera();

    void Tick(const glm::vec2& mouseOffset);
    glm::mat4 GetViewMatrix(const glm::vec3& position);
    glm::mat4 GetProjectionMatrix();
private:
    void updateCameraVectors();
    void processMouseMouvement(const glm::vec2& mouseOffset);
    void updateProjectionMatrix(const glm::vec2& screen);

    glm::mat4 _projectionMatrix;
	glm::vec3 _front;
	glm::vec3 _up;
	glm::vec3 _right;
	glm::vec3 _worldUp;
    float _yaw;
    float _pitch;

};