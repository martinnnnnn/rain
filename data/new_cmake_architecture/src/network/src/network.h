#pragma once

#include <string>
#include <iostream>

#include "thread.h"

namespace rain::network
{
    struct Network
    {
        std::string oh;
        thread::Thread* t;
    };

    void init_network(Network* _network)
    {
        std::cout << "init network" << std::endl;
        _network->oh = "OMG";
        _network->t = thread::get_new_thread();
        thread::init_thread(_network->t);
    }

    Network* get_new_network()
    {
        std::cout << "new network" << std::endl;
        return new Network();
    }
}