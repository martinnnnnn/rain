#pragma once


#include <algorithm>
#include <random>
#include <chrono>

namespace rain::core
{
    //void generate_random(size_t len, u8* buffer)
    //{
    //    static std::random_device randomDevice;
    //    static std::mt19937 randomEngine(randomDevice());

    //    std::uniform_int_distribution<u8> distribution;

    //    std::generate(buffer, buffer + len, [&]() { return distribution(randomEngine); });
    //}

    inline int generate_random_u32()
    {
        static unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
        static std::default_random_engine generator(seed);

        std::uniform_int_distribution<int> distribution(1, 100);

        return distribution(generator);
    }
}