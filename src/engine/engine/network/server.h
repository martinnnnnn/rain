#pragma once

#include "core/core.h"

#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <strsafe.h>
#include <stdlib.h>
#include <stdio.h>

namespace rain::engine
{
    namespace network
    {
        typedef struct
        {
            std::string address;
        } ip, *ip_ptr;

        enum class packet_type
        {
            world_update,
            textual_chat,
            vocal_chat
        };

        i32 start_server_thread(const char* ip_address);
        DWORD _stdcall start_server(LPVOID lpParam);
    }
}