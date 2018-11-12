#pragma once


#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

#include "core/types.h"
#include "physics/rigid_body.h"

namespace rain
{
    struct BoundingSphere
    {
        BoundingSphere()
            : position(glm::vec3(0.0f, 0.0f, 0.0f))
            , radius(0.0f)
        {}

        glm::vec3 position;
        f32 radius;
    };

    struct BoundingPlane
    {
        glm::vec3 position;
        glm::vec3 normal;
    };

    struct BoundingQuad
    {

    };

    struct BoundingBox
    {
    };

    struct collision
    {

    };

    glm::vec3 project(const glm::vec3& u, const glm::vec3& v)
    {
        return v * (glm::dot(u, v) / glm::dot(v, v));
    }


	bool detect_collision(BoundingSphere& _bound1, BoundingSphere& _bound2, RigidBody& _body1, RigidBody& _body2)
	{
		_bound1.position = _body1.position;
		_bound2.position = _body2.position;

		float length = glm::length(_body1.position - _body2.position);
		float sumradius = _bound1.radius + _bound2.radius;

		if (length <= sumradius)
		{
			return true;
		}

		return false;
	}

	void collision_response(BoundingSphere& _bound1, BoundingSphere& _bound2, RigidBody& _body1, RigidBody& _body2)
	{
		glm::vec3 U1x, U1y, U2x, U2y, V1x, V1y, V2x, V2y;


		f32 m1, m2, x1, x2;
		glm::vec3 v1temp, v1, v2, v1x, v2x, v1y, v2y;

		glm::vec3 x = glm::normalize(_body1.position - _body2.position);
		
		v1 = _body1.velocity;
		x1 = glm::dot(x, v1);
		v1x = x * x1;
		v1y = v1 - v1x;
		m1 = _body1.mass;

		x = x * -1.0f;
		v2 = _body2.velocity;
		x2 = glm::dot(x, v2);
		v2x = x * x2;
		v2y = v2 - v2x;
		m2 = _body2.mass;

		_body1.momentum = glm::vec3(v1x*(m1 - m2) / (m1 + m2) + v2x * (2 * m2) / (m1 + m2) + v1y) * _body1.mass;
		_body2.momentum = glm::vec3(v1x*(2 * m1) / (m1 + m2) + v2x * (m2 - m1) / (m1 + m2) + v2y) * _body2.mass;
	}

	//bool collision_detect_advanced(BoundingSphere& _bound1, BoundingSphere& _bound2, RigidBody& _body1, RigidBody& _body2)
	//{
	//	glm::vec3 s = _body1.position - _body2.position; // vector between the centers of each sphere
	//	glm::vec3 v = _body1.velocity - _body2.velocity; // relative velocity between spheres
	//	float r = _bound1.radius + _bound2.radius;

	//	float c1 = glm::dot(s, s) - r * r; // if negative, they overlap
	//	if (c1 < 0.0) // if true, they already overlap
	//	{
	//		// This is bad ... we need to correct this by moving them a tiny fraction from each other
	//		//a->pos +=
	//		t = .0;
	//		return true;
	//	}

	//	float a1 = glm::dot(v, v);
	//	if (a1 < 0.00001f)
	//		return false; // does not move towards each other

	//	float b1 = glm::dot(v, v);
	//	if (b1 >= 0.0)
	//		return false; // does not move towards each other

	//	float d1 = b1 * b1 - a1 * c1;
	//	if (d1 < 0.0)
	//		return false; // no real roots ... no collision

	//	t = (-b1 - sqrtf(d1)) / a1;

	//	return true;
	//}
}