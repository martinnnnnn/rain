#pragma once


#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <vector>

#include "core/types.h"

namespace rain
{

    struct RigidBody
    {
        RigidBody()
            : position(glm::vec3(0.0f, 0.0f, 0.0f))
            , momentum(glm::vec3(0.0f, 0.0f, 0.0f))
            , velocity(glm::vec3(0.0f, 0.0f, 0.0f))
            , force(glm::vec3(0.0f, 0.0f, 0.0f))
            , mass(1.0f)
            , mass_inverse(1.0f / mass)
        {}

        glm::vec3 position;
        glm::vec3 momentum;
        glm::vec3 velocity;
        glm::vec3 force;

        float mass;
        float mass_inverse;
    };

    void update(RigidBody& _body, float _deltaTime);
    void init_body(RigidBody& _body, const glm::vec3& _initialPosition = glm::vec3(0.0f, 0.0f, 0.0f));
    void update_body(RigidBody& _body, float _deltaTime, const std::vector<glm::vec3>& _forces);
}