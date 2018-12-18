#pragma once


#include <winsock2.h>
#include <stdio.h>
#pragma comment (lib, "Ws2_32.lib")

#include "core/types.h"
#include "core/logger.h"
#include "math/transform.h"

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

    //void archive(const Transform& t, char* _buffer);

#define PACK( __Declaration__ ) __pragma( pack(push, 1) ) __Declaration__ __pragma( pack(pop) )
    const int MaxDataSize = 1082;

    class Packet
    {
    public:
        int senderId;
        int sequenceNumber;
        char data[MaxDataSize];
    public:
        void* Serialize();
        void Deserialize(char *message);
    };

    PACK(
        struct SerializedPacket {
        int senderId;
        int sequenceNumber;
        char data[MaxDataSize];
    });
}