#include "collision.h"



namespace rain
{
    bool detect_collision(BoundingSphere& _bound1, BoundingSphere& _bound2, RigidBody& _body1, RigidBody& _body2)
    {
        //_bound1.position = _body1.position;
        //_bound2.position = _body2.position;

        //float length = glm::length(_body1.position - _body2.position);
        //float sumradius = _bound1.radius + _bound2.radius;

        //if (length <= sumradius)
        //{
        //    return true;
        //}

        return false;
    }

    void collision_response(BoundingSphere& _bound1, BoundingSphere& _bound2, RigidBody& _body1, RigidBody& _body2)
    {
        //glm::vec3 U1x, U1y, U2x, U2y, V1x, V1y, V2x, V2y;


        //f32 m1, m2, x1, x2;
        //glm::vec3 v1temp, v1, v2, v1x, v2x, v1y, v2y;

        //glm::vec3 x = glm::normalize(_body1.position - _body2.position);

        //v1 = _body1.velocity;
        //x1 = glm::dot(x, v1);
        //v1x = x * x1;
        //v1y = v1 - v1x;
        //m1 = _body1.mass;

        //x = x * -1.0f;
        //v2 = _body2.velocity;
        //x2 = glm::dot(x, v2);
        //v2x = x * x2;
        //v2y = v2 - v2x;
        //m2 = _body2.mass;

        //_body1.momentum = glm::vec3(v1x*(m1 - m2) / (m1 + m2) + v2x * (2 * m2) / (m1 + m2) + v1y) * _body1.mass;
        //_body2.momentum = glm::vec3(v1x*(2 * m1) / (m1 + m2) + v2x * (m2 - m1) / (m1 + m2) + v2y) * _body2.mass;
    }
}