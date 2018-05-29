#pragma once

#include <iostream>

#include "graphics/material.h"
#include "graphics/mesh.h"
#include "graphics/model.h"

namespace rain
{
	struct GameObject
	{
		
		void Init(const std::string& _meshPath, const std::string& _shaderPath)
		{
			Model model(_meshPath);
			m_mesh = model.m_meshes[0];

			m_material.Init(_shaderPath, )
		}
		
		Mesh m_mesh;
		Material m_material;
	};
}