#pragma once

#include <string>
#include <iostream>
#include <Windows.h>

#include "network/src/network.h"


namespace rain::engine
{
    void test()
    {
        std::cout << "test engine" << std::endl;
        network::Network* n = network::get_new_network();
        network::init_network(n);
    }
}

int WINAPI WinMain(HINSTANCE hinstance, HINSTANCE hprevinstance, LPSTR lpcmdline, int nshowcmd)
{
    rain::engine::test();
    return 0;
}