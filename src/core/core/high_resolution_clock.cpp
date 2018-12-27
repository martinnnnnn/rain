#include "high_resolution_clock.h"


namespace rain::core
{
    HighResolutionClock::HighResolutionClock()
        : m_deltaTime(0)
        , m_totalTime(0)
    {
        m_t0 = std::chrono::high_resolution_clock::now();
    }

    void HighResolutionClock::tick()
    {
        auto t1 = std::chrono::high_resolution_clock::now();
        m_deltaTime = t1 - m_t0;
        m_totalTime += m_deltaTime;
        m_t0 = t1;
    }

    void HighResolutionClock::reset()
    {
        m_t0 = std::chrono::high_resolution_clock::now();
        m_deltaTime = std::chrono::high_resolution_clock::duration();
        m_totalTime = std::chrono::high_resolution_clock::duration();
    }

    double HighResolutionClock::get_delta_nanoseconds() const
    {
        return m_deltaTime.count() * 1.0;
    }
    double HighResolutionClock::get_delta_microseconds() const
    {
        return m_deltaTime.count() * 1e-3;
    }

    double HighResolutionClock::get_delta_milliseconds() const
    {
        return m_deltaTime.count() * 1e-6;
    }

    double HighResolutionClock::get_delta_seconds() const
    {
        return m_deltaTime.count() * 1e-9;
    }

    double HighResolutionClock::get_total_nanoseconds() const
    {
        return m_totalTime.count() * 1.0;
    }

    double HighResolutionClock::get_total_microseconds() const
    {
        return m_totalTime.count() * 1e-3;
    }

    double HighResolutionClock::get_total_milliseconds() const
    {
        return m_totalTime.count() * 1e-6;
    }

    double HighResolutionClock::get_total_seconds() const
    {
        return m_totalTime.count() * 1e-9;
    }
}

