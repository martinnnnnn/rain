#pragma once

#include "glm.hpp"
#include <vector>
#include "float.h"

#include "core/core.h"
#include "engine/voxel2/volume_sample.h"

namespace rain::engine::voxel2
{
	typedef glm::ivec3 ivec3;
	typedef glm::uvec3 uvec3;

	struct ivec3_hasher
	{
		std::size_t operator()(const ivec3& position) const
		{
			std::size_t seed = 3;

			seed ^= position.x + 0x9e3779b9 + (seed << 6) + (seed >> 2);
			seed ^= position.y + 0x9e3779b9 + (seed << 6) + (seed >> 2);
			seed ^= position.z + 0x9e3779b9 + (seed << 6) + (seed >> 2);

			return seed;
		}

		bool operator()(const ivec3& position_a, const ivec3& position_b) const
		{
			return position_a.x == position_b.x
				&& position_a.y == position_b.y
				&& position_a.z == position_b.z;
		}

	};

	struct VolumeChunk
	{
		struct Size
		{
			Size(u32 size)
				: value(size)
				, squared(size * size)
				, cubed(size * size * size)
			{}

			const u32 value;
			const u32 squared;
			const u32 cubed;
		};

		static constexpr u32 DEFAULT_SIZE = 32;

		VolumeChunk(ivec3 i, u32 s = DEFAULT_SIZE)
			: index(i)
			, size(s)
		{
			samples.resize(size.cubed);
		}

		inline const Sample& get(u32 offset) const { return samples[offset]; }
		inline const Sample& get(u32 x, u32 y, u32 z) const { return get(x + y * size.value + z * size.squared); }
		inline const Sample& get(uvec3 position) const { return get(position.x, position.y, position.z); }

		inline void set(const Sample& sample, u32 offset) { samples[offset] = sample; }
		inline void set(const Sample& sample, u32 x, u32 y, u32 z) { set(sample, x + y * size.value + z * size.squared); }
		inline void set(const Sample& sample, uvec3 position) { set(sample, position.x, position.y, position.z); }

		ivec3				index;
		Size			    size;
		std::vector<Sample> samples;
	};
}


