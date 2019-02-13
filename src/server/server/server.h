#pragma once

#include "core/core.h"

#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <strsafe.h>
#include <stdlib.h>
#include <stdio.h>

namespace rain::server
{

    typedef struct
    {
        std::string address;
    } ip, *ip_ptr;

    i32 init_winsock();
    i32 shutdown_winsock();

    struct server
    {
        i32 init(const char* ip, const char* port);
        i32 close_server();
        i32 start_listening();
        DWORD launch_client_socket();

        static DWORD WINAPI launch_client_socket_static(void* Param);

        SOCKET listen_socket;
        std::vector<SOCKET> client_sockets;
    };
}
