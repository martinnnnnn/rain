#include "network.h"


namespace rain::core
{
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

        printf("usage: %s server-name\n", "server_test");

        i_result = WSAStartup(MAKEWORD(2, 2), &wsa_data);
        if (i_result != 0)
        {
            printf("WSAStartup failed with error: %d\n", i_result);
            return 1;
        }

        ZeroMemory(&hints, sizeof(hints));
        hints.ai_family = AF_UNSPEC;
        hints.ai_socktype = SOCK_STREAM;
        hints.ai_protocol = IPPROTO_TCP;

        i_result = getaddrinfo(ip_address, port, &hints, &result);
        if (i_result != 0)
        {
            printf("getaddrinfo failed with error: %d\n", i_result);
            WSACleanup();
            return 1;
        }

        for (ptr = result; ptr != NULL; ptr = ptr->ai_next)
        {

            connect_socket = socket(ptr->ai_family, ptr->ai_socktype,
                ptr->ai_protocol);
            if (connect_socket == INVALID_SOCKET) {
                printf("socket failed with error: %ld\n", WSAGetLastError());
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
            printf("Unable to connect to server!\n");
            WSACleanup();
            return 1;
        }

        char input[512];
        printf("You : ");
        fgets(input, sizeof(input), stdin);
        while (strcmp(input, "exit") != 0)
        {
            i_result = send(connect_socket, input, (int)strlen(input), 0);
            if (i_result == SOCKET_ERROR)
            {
                printf("send failed with error: %d\n", WSAGetLastError());
                closesocket(connect_socket);
                WSACleanup();
                return 1;
            }

            printf("You : ");
            fgets(input, sizeof(input), stdin);
        }

        i_result = shutdown(connect_socket, SD_SEND);
        if (i_result == SOCKET_ERROR)
        {
            printf("shutdown failed with error: %d\n", WSAGetLastError());
            closesocket(connect_socket);
            WSACleanup();
            return 1;
        }

        do {

            i_result = recv(connect_socket, recvbuf, recvbuflen, 0);
            if (i_result > 0)
            {
                printf("Bytes received: %d\n", i_result);
            }
            else if (i_result == 0)
            {
                printf("Connection closed\n");
            }
            else
            {
                printf("recv failed with error: %d\n", WSAGetLastError());
            }

        } while (i_result > 0);

        closesocket(connect_socket);
        WSACleanup();

        return 0;
    }
}