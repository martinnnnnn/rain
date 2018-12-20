#include "thread.h"

namespace rain::thread
{

    void init_thread(Thread* _thread)
    {
        std::cout << "init thread from cpp" << std::endl;
        _thread->hello = 1;
        _thread->hi = 42.0;
        _thread->hello = 'c';
    }

    rain::thread::Thread* get_new_thread()
    {
        std::cout << "new thread from cpp" << std::endl;
        return new Thread();
    }

}