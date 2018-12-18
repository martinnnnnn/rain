#pragma once


#include <winsock2.h>
#include <stdio.h>
#pragma comment (lib, "Ws2_32.lib")

#include "core/types.h"
#include "core/logger.h"

namespace rain
{
    struct Client
    {
        SOCKET socket;
        struct sockaddr_in serverInfo;
        int serverInfoLenght;
    };

    void init_socket(Client* _client);

    void close_socket(Client* _client);

    void send_data(Client* _client, const char* _buffer, u32 _size);

    void check_receive_data(Client* _client, char* _buffer, u32 _size);
}