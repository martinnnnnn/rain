#pragma once

#include "math/numbers/math_basics.h"
#include "math/geometry/primitives/vec3.h"
#include "math/geometry/primitives/plane.h"

#include <vector>

namespace rain::math
{

	struct gjk_mesh
	{
		vec3* vertices;
		u32 nvertices;
		u32* indices;
		u32 nindices;
	};


	vec3 get_first_direction(const gjk_mesh& mesh_a, const gjk_mesh& mesh_b)
	{
		return vec3{ 1.0f, 0.0f, 0.0f };
	}

	vec3 farthest_in_direction(const gjk_mesh& mesh, const vec3& direction)
	{
		vec3 far_vertex = mesh.vertices[0];
		f32 far_distance = 0;
		for (u32 i = 1; i < mesh.nvertices; ++i)
		{
			f32 current_distance = dot(direction, mesh.vertices[i]);
			if (current_distance > far_distance)
			{
				far_vertex = mesh.vertices[i];
				far_distance = current_distance;
			}
		}
		return far_vertex;
	}

	vec3 support(const gjk_mesh& mesh_a, const gjk_mesh& mesh_b, const vec3& direction)
	{
		vec3 a = farthest_in_direction(mesh_a, direction);
		vec3 b = farthest_in_direction(mesh_a, -direction);
		return a - b;
	}

	struct gjk_simplex
	{
		vec3 vertices[4];
	};

	void build_simplex(const gjk_mesh& mesh_a, const gjk_mesh& mesh_b, gjk_simplex& simplex)
	{
		simplex.vertices[0] = support(mesh_a, mesh_b, vec3{ 1.0f, 0.0f, 0.0f });
		simplex.vertices[1] = support(mesh_a, mesh_b, vec3{ 0.0f, 1.0f, 0.0f });
		simplex.vertices[2] = support(mesh_a, mesh_b, vec3{ 0.0f, 0.0f, 1.0f });
		Plane plane(simplex.vertices[0], simplex.vertices[1], simplex.vertices[2]);
		simplex.vertices[3] = support(mesh_a, mesh_b, plane.n);
	}

	bool is_colliding_gjk(const gjk_mesh& mesh_a, const gjk_mesh& mesh_b)
	{
		gjk_simplex simplex;
		build_simplex(mesh_a, mesh_b, simplex);

		
	}
}