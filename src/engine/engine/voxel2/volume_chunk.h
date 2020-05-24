#pragma once

#include "glm.hpp"
#include <vector>
#include "float.h"

#include "core/core.h"
#include "engine/voxel2/volume_sample.h"

namespace rain::engine::voxel2
{
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

			inline u32		get_offset(u32 x, u32 y, u32 z) { return x + y * value + z * squared; }
			inline u32		get_offset(const uvec3& position) { return get_offset(position.x, position.y, position.z); }
			inline uvec3	get_position(u32 offset)
			{
				const u32 z = offset / squared;
				const u32 y = (offset - z * squared) / value;
				const u32 x = (offset - z * squared - y * value);

				return uvec3 { x, y, z };
			}
		};

		static constexpr u32 DEFAULT_SIZE = 32;

		VolumeChunk(ivec3 i, u32 s = DEFAULT_SIZE)
			: index(i)
			, size(s)
		{
			samples.resize(size.cubed);
		}

		inline i32 offset_from_absolute(i32 x, i32 y, i32 z) const;

		inline const Sample& get(u32 offset) const { return samples[offset]; }
		inline const Sample& get_local(u32 x, u32 y, u32 z) const { return get(x + y * size.value + z * size.squared); }
		inline const Sample& get_local(const uvec3& position) const { return get_local(position.x, position.y, position.z); }

		inline const Sample& get_absolute(i32 x, i32 y, i32 z) const { return get(offset_from_absolute(x, y, z)); }
		inline const Sample& get_absolute(const uvec3& position) const { return get(offset_from_absolute(position.x, position.y, position.z)); }



		inline void set(const Sample& sample, u32 offset) { samples[offset] = sample; }
		inline void set(const Sample& sample, u32 x, u32 y, u32 z) { set(sample, x + y * size.value + z * size.squared); }
		inline void set(const Sample& sample, uvec3 position) { set(sample, position.x, position.y, position.z); }

		ivec3				index;
		Size			    size;
		std::vector<Sample> samples;
	};

	i32 VolumeChunk::offset_from_absolute(i32 x, i32 y, i32 z) const
	{
		x = index.x < 0 ? x - 1 : x;
		y = index.y < 0 ? y - 1 : y;
		z = index.z < 0 ? y - 1 : z;

		const i32 xoff = (x - index.x * size.value);
		const i32 yoff = (y - index.y * size.value) * size.value;
		const i32 zoff = (z - index.z * size.value) * size.squared;

		return xoff + yoff + zoff;
	}
}


