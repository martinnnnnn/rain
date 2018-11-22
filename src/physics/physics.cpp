#include "physics.h"


#include "math/transform.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/trigonometric.hpp>

#include "gfx/ogl/ogl_renderer.h"

namespace rain
{

    void Physics::apply_gravity(RigidBody& _body)
    {
        if (_body.applyGravity)
        {
            _body.force += glm::vec3(0.0f, -9.81f, 0.0f);
        }
    }

    void Physics::apply_spring(const Spring& _spring, const Transform& _transformA, RigidBody& _bodyA, const Transform& _transformB, RigidBody& _bodyB)
    {

        glm::vec3 position_anchorA = glm::translate(glm::mat4(1), _transformA.position) * glm::mat4_cast(_transformA.orientation) * glm::vec4(_spring.anchorPointA, 1.0);
        glm::vec3 position_anchorB = glm::translate(glm::mat4(1), _transformB.position) * glm::mat4_cast(_transformB.orientation) * glm::vec4(_spring.anchorPointB, 1.0);

        RAIN_RENDERER->draw_debug_line(position_anchorA, position_anchorB, glm::vec3(0.0f, 1.0f, 0.0f));

        glm::vec3 point_velocityA = _bodyA.velocity + glm::cross(_bodyA.angularVelocity, (position_anchorA, _transformA.position));
        glm::vec3 point_velocityB = _bodyB.velocity + glm::cross(_bodyB.angularVelocity, (position_anchorB, _transformB.position));

        f32 distance = glm::distance(position_anchorA, position_anchorB);
        glm::vec3 directionAtoB = glm::normalize(position_anchorB - position_anchorA);
        //glm::vec3 spring_forceA = -_spring.k * (distance - _spring.distance) * -directionAtoB - _spring.b * point_velocityA;
        //glm::vec3 spring_forceB = -_spring.k * (distance - _spring.distance) * directionAtoB - _spring.b * point_velocityB;

        glm::vec3 spring_forceA = -_spring.k * (_transformA.position - _spring.anchorPointA);
        glm::vec3 spring_forceB = -_spring.k * (_transformB.position - _spring.anchorPointB);
        glm::vec3 VUa = _bodyA.velocity + glm::cross(_bodyA.angularVelocity, position_anchorA);
        glm::vec3 VUb = _bodyB.velocity + glm::cross(_bodyB.angularVelocity, position_anchorB);
        glm::vec3 dampingA = -_spring.b * (glm::dot(VUa, spring_forceA) / glm::dot(spring_forceA, spring_forceA)) * spring_forceA;
        glm::vec3 dampingB = -_spring.b * (glm::dot(VUb, spring_forceB) / glm::dot(spring_forceB, spring_forceB)) * spring_forceB;
        spring_forceA += dampingA;
        spring_forceB += dampingB;

        //vector_3 VU = Configuration.CMVelocity +
        //    CrossProduct(Configuration.AngularVelocity,  Position - Configuration.CMPosition);
        //vector_3 Spring = -Kws * (Position - SpringStructure.Anchor);
        //vector_3 DampingForce =
        //    -Kwd * (DotProduct(VU, Spring) / DotProduct(Spring, Spring)) * Spring;


        _bodyA.force += spring_forceA;
        _bodyB.force += spring_forceB;

        _bodyA.torque += glm::cross(spring_forceA, (position_anchorA, _transformA.position));
        _bodyB.torque += glm::cross(spring_forceB, (position_anchorB, _transformB.position));

        //char buffer[500];
        //sprintf_s(buffer, 500, "torqueA : (%f,%f,%f), torqueB : (%f,%f,%f)\n", _bodyA.torque.x, _bodyA.torque.y, _bodyA.torque.z, _bodyB.torque.x, _bodyB.torque.y, _bodyB.torque.z);
        //OutputDebugString(buffer);
    }


    void Physics::update(RigidBody& _body, Transform& _transform, float _deltaTime)
    {
        _transform.previousPosition = _transform.position;
        _transform.previousOrientation = _transform.orientation;

        apply_physics(_body, _transform, _deltaTime);
    }
}