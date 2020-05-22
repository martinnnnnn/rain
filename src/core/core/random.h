#pragma once


#include <time.h>
#include <stdlib.h>

#include "core/types.h"

namespace rain::core
{
    //void generate_random(size_t len, u8* buffer)
    //{
    //    static std::random_device randomDevice;
    //    static std::mt19937 randomEngine(randomDevice());

    //    std::uniform_int_distribution<u8> distribution;

    //    std::generate(buffer, buffer + len, [&]() { return distribution(randomEngine); });
    //}

	struct Random
	{
		inline void initialize()
		{
			srand(u32(time(NULL)));
		}

		inline u32 range(u32 min, u32 max)
		{
			return min + (rand() % (max - min));
		}

		inline u32 range_0X(u32 max)
		{
			return range(0, max);
		}

		inline f32 range_f01()
		{
			return f32(rand() % 100) / 100.0f;
		}

		inline f32 range_f(f32 min, f32 max)
		{
			return range_f01() * (max - min) + min;
		}

		inline f32 range_f0X(f32 max)
		{
			return range_f(0.0f, max);
		}
	};

    //inline int generate_random_u32()
    //{
    //    static unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    //    static std::default_random_engine generator(seed);

    //    std::uniform_int_distribution<int> distribution(1, 100);

    //    return distribution(generator);
    //}
}