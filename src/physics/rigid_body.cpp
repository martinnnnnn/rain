#include "rigid_body.h"

#include <Windows.h>


namespace rain
{
    void update(RigidBody& _body, float _deltaTime)
    {
        std::vector<glm::vec3> forces;
        forces.push_back(glm::vec3(0.0f, -9.81f, 0.0f));

        update_body(_body, _deltaTime, forces);
    }

    void init_body(RigidBody& _body, const glm::vec3& _initialPosition)
    {
        _body.position = _initialPosition;
        _body.momentum = glm::vec3(5.0f, 15.0f, 0.0f);
        _body.mass = 3.0f;
        _body.mass_inverse = 1.0f / _body.mass;
    }

    void update_body(RigidBody& _body, float _deltaTime, const std::vector<glm::vec3>& _forces)
    {
        _body.force = glm::vec3(0.0f);
        for (int i = 0; i < _forces.size(); ++i)
        {
            _body.force += _forces[i];
        }

        _body.momentum += _body.force * _deltaTime;
        _body.velocity = _body.momentum * _body.mass_inverse;
        _body.position += _body.velocity * _deltaTime;
    }

}