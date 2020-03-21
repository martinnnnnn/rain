#pragma once

#include <chrono>
#include <string>



namespace rain::core
{
    class HighResolutionClock
    {
    public:
        HighResolutionClock();

        void tick();

        void reset();

        double get_delta_nanoseconds() const;
        double get_delta_microseconds() const;
        double get_delta_milliseconds() const;
        double get_delta_seconds() const;

        double get_total_nanoseconds() const;
        double get_total_microseconds() const;
        double get_total_milliseconds() const;
        double get_total_seconds() const;

    private:

        std::chrono::high_resolution_clock::time_point m_t0;

        std::chrono::high_resolution_clock::duration m_deltaTime;
        std::chrono::high_resolution_clock::duration m_totalTime;
    };
}