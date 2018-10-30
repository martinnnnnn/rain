#pragma once


#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>
#include <vector>

#include "core/types.h"

namespace rain
{

    struct RigidBody
    {
        glm::vec3 position;
        glm::vec3 momentum;
        glm::vec3 velocity;
        glm::vec3 force;

        float mass;
        float mass_inverse;
    };

    void update(RigidBody _body, float _deltaTime);
    void init_body(RigidBody _body);
    void update_body(RigidBody& _body, float _deltaTime, const std::vector<glm::vec3>& _forces);




//    struct RigidBodyRK4
//    {
//        // primary
//        glm::vec3 position;
//        glm::vec3 momentum;
//
//        // secondary
//        glm::vec3 velocity;
//
//        // constants
//        float mass;
//        float inverseMass;
//
//        void recalculate()
//        {
//            velocity = momentum * inverseMass;
//        }
//    };
//
//    struct DerivativeRK4
//    {
//        glm::vec3 velocity;
//        glm::vec3 force;
//    };
//
//    struct State
//    {
//        float x;
//        float v;
//    };
//
//    struct Derivative
//    {
//        float dx;
//        float dv;
//    };
//
//    Derivative evaluate(const State & initial,
//        double t,
//        float dt,
//        const Derivative & d)
//    {
//        State state;
//        state.x = initial.x + d.dx*dt;
//        state.v = initial.v + d.dv*dt;
//
//        Derivative output;
//        output.dx = state.v;
//        output.dv = acceleration(state, t + dt);
//        return output;
//    }
//
//    float acceleration(const State & state, double t)
//    {
//        const float k = 15.0f;
//        const float b = 0.1f;
//        return -k * state.x - b * state.v;
//    }
//
//    void integrate(State & state,
//        double t,
//        float dt)
//    {
//        Derivative a, b, c, d;
//
//        a = evaluate(state, t, 0.0f, Derivative());
//        b = evaluate(state, t, dt*0.5f, a);
//        c = evaluate(state, t, dt*0.5f, b);
//        d = evaluate(state, t, dt, c);
//
//        float dxdt = 1.0f / 6.0f *
//            (a.dx + 2.0f * (b.dx + c.dx) + d.dx);
//
//        float dvdt = 1.0f / 6.0f *
//            (a.dv + 2.0f * (b.dv + c.dv) + d.dv);
//
//        state.x = state.x + dxdt * dt;
//        state.v = state.v + dvdt * dt;
//    }
}