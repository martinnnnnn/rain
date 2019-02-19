#pragma once

#include "core/core.h"
#include <vector>

#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <strsafe.h>
#include <stdlib.h>
#include <stdio.h>


#include <thread>



namespace rain::server
{




    typedef struct
    {
        std::string address;
    } ip, *ip_ptr;

    i32 init_winsock();
    i32 shutdown_winsock();
    void print_error(i32 code, const char* fn);

    struct client_info
    {
        i32 id;
        SOCKET socket;
        std::string name;
    };

    struct context : core::Singleton<context>
    {
        core::logger* logger;
    };


    struct server
    {
        i32 init(const char* ip, const char* port);
        i32 close_server();
        i32 start_listening();
        DWORD launch_client_socket();

        static DWORD WINAPI launch_client_socket_static(void* Param);
        static DWORD WINAPI launch_listening_static(void* Param);

        SOCKET listen_socket;
        std::vector<SOCKET> client_sockets;
    };
}

#define RAIN_SERVER_CONTEXT                     rain::server::context::get()
#define RAIN_LOG_SERVER(...)                    do { RAIN_SERVER_CONTEXT->logger->log_level("[SERVER]", __VA_ARGS__); } while(false)

