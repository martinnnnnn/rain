#pragma once

#include <iostream>

namespace rain::thread
{
    struct Thread
    {
        int hello;
        double hi;
        char dam;
    };

    void init_thread(Thread* _thread);

    Thread* get_new_thread();
}