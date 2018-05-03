#pragma once


//#include <atomic>
//#define FIRST_TIME_HERE ([] { \
//    static std::atomic<bool> first_time(true); \
//    return first_time.exchange(false); } ())

#define FIRST_TIME_HERE ([] { \
    static bool is_first_time = true; \
    bool was_first_time = is_first_time; \
    is_first_time = false; \
    return was_first_time; } ())