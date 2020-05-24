#pragma once

#include "core/core.h"

#include "engine/win32/win32_helpers.h"
#include "network.h"

namespace rain::engine
{
    namespace network
    {
        typedef struct
        {
            std::string address;
        } ip, *ip_ptr;


        i32 start_server_thread(const char* ip_address);
        i32 start_server_thread(const connexion_info& info);
        DWORD _stdcall start_server(LPVOID lpParam);
        DWORD _stdcall start_server_from_socket(LPVOID lpParam);
    }
}