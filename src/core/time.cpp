#include "time.h"




namespace rain
{
    Time::Time()
    {
        QueryPerformanceFrequency(&m_frequency);
    }

    __int64 Time::GetTime() const
    {
        return  GetCount() / m_frequency.QuadPart;
    }

    __int64 Time::GetCount() const
    {
        LARGE_INTEGER count = { 0 };
        QueryPerformanceCounter(&count);
        return count.QuadPart;
    }

    __int64 Time::GetFrequency() const
    {
        return m_frequency.QuadPart;
    }

    __int64 Time::GetTimeMs() const
    {
        return (1000 * GetCount()) / GetFrequency();
    }

}
