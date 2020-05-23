#pragma once

#include "core/core.h"

namespace rain::engine::voxel2
{
	struct Sample
	{
		Sample(i8 val) : value(val) {}
		Sample() : Sample(0) {}

		i8 value;
	};
}


