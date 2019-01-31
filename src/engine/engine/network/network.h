#pragma once


#include "core/core.h"

#define DEFAULT_BUFLEN 512

namespace rain::engine
{
    namespace network
    {
        i32 init();
        void terminate();
    }
    int start_tcp_client(const char* ip_address, const char* port);
}