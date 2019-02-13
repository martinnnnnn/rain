#include "network.h"

#include "engine/core/context.h"


namespace rain::engine::network
{
    
    i32 init()
    {
        WSADATA wsa_data;
        i32 i_result = WSAStartup(MAKEWORD(2, 2), &wsa_data);
        if (i_result != 0)
        {
            RAIN_LOG_NETWORK("WSAStartup failed with error: %d\n", i_result);
            return 1;
        }
        return 0;
    }

    void terminate()
    {
        WSACleanup();
    }


    int start_tcp_client(connexion_info& info, const char* ip_address, const char* port)
    {
        info.socket = INVALID_SOCKET;
        struct addrinfo *result = NULL,
            *ptr = NULL,
            hints;
        const char *sendbuf = "this is a test\n";
        int i_result;

        RAIN_LOG_NETWORK("usage: %s server-name\n", "server_test");

        ZeroMemory(&hints, sizeof(hints));
        hints.ai_family = AF_UNSPEC;
        hints.ai_socktype = SOCK_STREAM;
        hints.ai_protocol = IPPROTO_TCP;

        i_result = getaddrinfo(ip_address, port, &hints, &result);
        if (i_result != 0)
        {
            RAIN_LOG_NETWORK("getaddrinfo failed with error: %d\n", i_result);
            return 1;
        }

        for (ptr = result; ptr != NULL; ptr = ptr->ai_next)
        {
            info.socket = socket(ptr->ai_family, ptr->ai_socktype,
                ptr->ai_protocol);
            if (info.socket == INVALID_SOCKET)
            {
                RAIN_LOG_NETWORK("socket failed with error: %ld\n", WSAGetLastError());
                return 1;
            }

            i_result = connect(info.socket, ptr->ai_addr, (int)ptr->ai_addrlen);
            if (i_result == SOCKET_ERROR)
            {
                closesocket(info.socket);
                info.socket = INVALID_SOCKET;
                continue;
            }
            break;
        }
        freeaddrinfo(result);

        if (info.socket == INVALID_SOCKET)
        {
            RAIN_LOG_NETWORK("Unable to connect to server!\n");
            return 1;
        }

        return 0;
    }

    i32 stop_tcp_client(const connexion_info& info)
    {
        i32 result = shutdown(info.socket, SD_SEND);
        if (result == SOCKET_ERROR)
        {
            RAIN_LOG_NETWORK("shutdown failed with error: %d\n", WSAGetLastError());
            return 1;
        }

        char buffer[DEFAULT_BUFLEN];
        do {
            result = recv(info.socket, buffer, DEFAULT_BUFLEN, 0);
            if (result > 0)
            {
                RAIN_LOG_NETWORK("Bytes received: %d\n", result);
                RAIN_LOG_NETWORK("Text received : %s", buffer);
            }
            else if (result == 0)
            {
                RAIN_LOG_NETWORK("Connection closed\n");
            }
            else
            {
                RAIN_LOG_NETWORK("recv failed with error: %d\n", WSAGetLastError());
            }

        } while (result > 0);
        closesocket(info.socket);
    }

    i32 send_packet(const connexion_info& info, const char* buffer)
    {
        RAIN_LOG_NETWORK("Send : %s", buffer);
        i32 i_result = send(info.socket, buffer, (i32)strlen(buffer), 0);
        if (i_result == SOCKET_ERROR)
        {
            RAIN_LOG_NETWORK("send failed with error: %d\n", WSAGetLastError());
            return -1;
        }
    }
}