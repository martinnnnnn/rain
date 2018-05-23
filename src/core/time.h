#pragma once


#include <Windows.h>

namespace rain
{

    class Time
    {
    public:
        Time();

        __int64         GetTime() const;
        __int64         GetCount() const;
        __int64         GetFrequency() const;
        __int64         GetTimeMs() const;

    private:
        LARGE_INTEGER   m_frequency;
    };

} 
