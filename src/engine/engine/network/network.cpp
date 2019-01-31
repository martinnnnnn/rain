#include "network.h"


#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>
#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "AdvApi32.lib")

#include "engine/core/context.h"


namespace rain::engine
{
    namespace network
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
        }

        void terminate()
        {
            WSACleanup();
        }
    }



    int start_tcp_client(const char* ip_address, const char* port)
    {
        WSADATA wsa_data;
        SOCKET connect_socket = INVALID_SOCKET;
        struct addrinfo *result = NULL,
            *ptr = NULL,
            hints;
        const char *sendbuf = "this is a test\n";
        char recvbuf[DEFAULT_BUFLEN];
        int i_result;
        int recvbuflen = DEFAULT_BUFLEN;

        RAIN_LOG("usage: %s server-name\n", "server_test");

        i_result = WSAStartup(MAKEWORD(2, 2), &wsa_data);
        if (i_result != 0)
        {
            RAIN_LOG("WSAStartup failed with error: %d\n", i_result);
            return 1;
        }

        ZeroMemory(&hints, sizeof(hints));
        hints.ai_family = AF_UNSPEC;
        hints.ai_socktype = SOCK_STREAM;
        hints.ai_protocol = IPPROTO_TCP;

        i_result = getaddrinfo(ip_address, port, &hints, &result);
        if (i_result != 0)
        {
            RAIN_LOG("getaddrinfo failed with error: %d\n", i_result);
            WSACleanup();
            return 1;
        }

        for (ptr = result; ptr != NULL; ptr = ptr->ai_next)
        {

            connect_socket = socket(ptr->ai_family, ptr->ai_socktype,
                ptr->ai_protocol);
            if (connect_socket == INVALID_SOCKET) {
                RAIN_LOG("socket failed with error: %ld\n", WSAGetLastError());
                WSACleanup();
                return 1;
            }

            i_result = connect(connect_socket, ptr->ai_addr, (int)ptr->ai_addrlen);
            if (i_result == SOCKET_ERROR) {
                closesocket(connect_socket);
                connect_socket = INVALID_SOCKET;
                continue;
            }
            break;
        }

        freeaddrinfo(result);

        if (connect_socket == INVALID_SOCKET)
        {
            RAIN_LOG("Unable to connect to server!\n");
            WSACleanup();
            return 1;
        }

        char input[512];
        RAIN_LOG("You : ");
        FILE* file = fopen("D:/_MARTIN/programming/_projects/editor/test_blazor_server/test.txt", "r");
        fgets(input, sizeof(input), file);
        while (strcmp(input, "exit") != 0)
        {
            RAIN_LOG("You : %s", input);
            i_result = send(connect_socket, input, (int)strlen(input), 0);
            if (i_result == SOCKET_ERROR)
            {
                RAIN_LOG("send failed with error: %d\n", WSAGetLastError());
                closesocket(connect_socket);
                WSACleanup();
                return 1;
            }

            
            if (!fgets(input, sizeof(input), file) || ferror(file) || feof(file)) break;
        }

        i_result = shutdown(connect_socket, SD_SEND);
        if (i_result == SOCKET_ERROR)
        {
            RAIN_LOG("shutdown failed with error: %d\n", WSAGetLastError());
            closesocket(connect_socket);
            WSACleanup();
            return 1;
        }

        do {

            i_result = recv(connect_socket, recvbuf, recvbuflen, 0);
            if (i_result > 0)
            {
                RAIN_LOG("Bytes received: %d\n", i_result);
                RAIN_LOG("Text received : %s", recvbuf);
            }
            else if (i_result == 0)
            {
                RAIN_LOG("Connection closed\n");
            }
            else
            {
                RAIN_LOG("recv failed with error: %d\n", WSAGetLastError());
            }

        } while (i_result > 0);

        closesocket(connect_socket);
        WSACleanup();

        return 0;
    }
}