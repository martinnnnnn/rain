#include "physics.h"



#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/trigonometric.hpp>

#include "engine/core/context.h"
#include "engine/gfx/ogl/ogl_renderer.h"

namespace rain::engine
{

    void Physics::apply_gravity(RigidBody& _body)
    {
        if (_body.applyGravity)
        {
            _body.force += vec3{ 0.0f, -9.81f, 0.0f };
        }
    }

    void Physics::apply_spring(const Spring& _spring, const Transform& _transformA, RigidBody& _bodyA, const Transform& _transformB, RigidBody& _bodyB)
    {

        vec3 position_anchorA = translate(mat4(1), _transformA.position) * mat4_cast(_transformA.orientation) * vec4(_spring.anchorPointA, 1.0);
        vec3 position_anchorB = translate(mat4(1), _transformB.position) * mat4_cast(_transformB.orientation) * vec4(_spring.anchorPointB, 1.0);

        RAIN_RENDERER->draw_debug_line(position_anchorA, position_anchorB, vec3{ 0.0f, 1.0f, 0.0f });

        vec3 point_velocityA = _bodyA.velocity + cross(_bodyA.angularVelocity, (position_anchorA, _transformA.position));
        vec3 point_velocityB = _bodyB.velocity + cross(_bodyB.angularVelocity, (position_anchorB, _transformB.position));

        f32 distance = distance(position_anchorA, position_anchorB);
        vec3 directionAtoB = normalized(position_anchorB - position_anchorA);
        //vec3 spring_forceA = -_spring.k * (distance - _spring.distance) * -directionAtoB - _spring.b * point_velocityA;
        //vec3 spring_forceB = -_spring.k * (distance - _spring.distance) * directionAtoB - _spring.b * point_velocityB;

        vec3 spring_forceA = -_spring.k * (_transformA.position - _spring.anchorPointA);
        vec3 spring_forceB = -_spring.k * (_transformB.position - _spring.anchorPointB);
        vec3 VUa = _bodyA.velocity + cross(_bodyA.angularVelocity, position_anchorA);
        vec3 VUb = _bodyB.velocity + cross(_bodyB.angularVelocity, position_anchorB);
        vec3 dampingA = -_spring.b * (dot(VUa, spring_forceA) / dot(spring_forceA, spring_forceA)) * spring_forceA;
        vec3 dampingB = -_spring.b * (dot(VUb, spring_forceB) / dot(spring_forceB, spring_forceB)) * spring_forceB;
        spring_forceA += dampingA;
        spring_forceB += dampingB;

        //vector_3 VU = Configuration.CMVelocity +
        //    CrossProduct(Configuration.AngularVelocity,  Position - Configuration.CMPosition);
        //vector_3 Spring = -Kws * (Position - SpringStructure.Anchor);
        //vector_3 DampingForce =
        //    -Kwd * (DotProduct(VU, Spring) / DotProduct(Spring, Spring)) * Spring;


        _bodyA.force += spring_forceA;
        _bodyB.force += spring_forceB;

        _bodyA.torque += cross(spring_forceA, (position_anchorA, _transformA.position));
        _bodyB.torque += cross(spring_forceB, (position_anchorB, _transformB.position));

        //RAIN_LOG("torqueA : (%f,%f,%f), torqueB : (%f,%f,%f)\n", _bodyA.torque.x, _bodyA.torque.y, _bodyA.torque.z, _bodyB.torque.x, _bodyB.torque.y, _bodyB.torque.z);

    }


    void Physics::update(RigidBody& _body, Transform& _transform, float _deltaTime)
    {
        _transform.lastPosition = _transform.position;
        _transform.lastOrientation = _transform.orientation;

        apply_physics(_body, _transform, _deltaTime);
    }
}