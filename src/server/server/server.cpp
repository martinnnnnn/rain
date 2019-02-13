#include "server.h"



#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "AdvApi32.lib")


#define DEFAULT_BUFLEN 512


namespace rain::server
{

    void ErrorHandler(LPTSTR lpszFunction)
    {
        LPVOID lpMsgBuf;
        LPVOID lpDisplayBuf;
        DWORD dw = GetLastError();

        FormatMessage(
            FORMAT_MESSAGE_ALLOCATE_BUFFER |
            FORMAT_MESSAGE_FROM_SYSTEM |
            FORMAT_MESSAGE_IGNORE_INSERTS,
            NULL,
            dw,
            MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
            (LPTSTR)&lpMsgBuf,
            0, NULL);

        // Display the error message.

        lpDisplayBuf = (LPVOID)LocalAlloc(LMEM_ZEROINIT,
            (lstrlen((LPCTSTR)lpMsgBuf) + lstrlen((LPCTSTR)lpszFunction) + 40) * sizeof(TCHAR));
        StringCchPrintf((LPTSTR)lpDisplayBuf,
            LocalSize(lpDisplayBuf) / sizeof(TCHAR),
            TEXT("%s failed with error %d: %s"),
            lpszFunction, dw, lpMsgBuf);
        MessageBox(NULL, (LPCTSTR)lpDisplayBuf, TEXT("Error"), MB_OK);

        // Free error-handling buffer allocations.

        LocalFree(lpMsgBuf);
        LocalFree(lpDisplayBuf);
    }

    i32 init_winsock()
    {
        WSADATA wsa_data;
        i32 i_result = WSAStartup(MAKEWORD(2, 2), &wsa_data);
        if (i_result != 0)
        {
            printf("WSAStartup failed with error: %d\n", i_result);
            return 1;
        }
        return 0;
    }

    i32 shutdown_winsock()
    {
        WSACleanup();
        return 0;
    }

    i32 server::init(const char* ip, const char* port)
    {
        int i_result;

        listen_socket = INVALID_SOCKET;

        struct addrinfo *result = NULL;
        struct addrinfo hints;

        ZeroMemory(&hints, sizeof(hints));
        hints.ai_family = AF_INET;
        hints.ai_socktype = SOCK_STREAM;
        hints.ai_protocol = IPPROTO_TCP;
        hints.ai_flags = AI_PASSIVE;

        //std::string ip;
        //std::string port;
        //core::string::pair_split(ip_address_and_port->address, ":", ip, port);

        // Resolve the server address and port
        i_result = getaddrinfo(ip, port, &hints, &result);
        if (i_result != 0) {
            printf("getaddrinfo failed with error: %d\n", i_result);
            return 1;
        }

        // Create a SOCKET for connecting to server
        listen_socket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
        if (listen_socket == INVALID_SOCKET) {
            printf("socket failed with error: %ld\n", WSAGetLastError());
            freeaddrinfo(result);
            return 1;
        }

        // Setup the TCP listening socket
        i_result = bind(listen_socket, result->ai_addr, (int)result->ai_addrlen);
        if (i_result == SOCKET_ERROR) {
            printf("bind failed with error: %d\n", WSAGetLastError());
            freeaddrinfo(result);
            closesocket(listen_socket);
            return 1;
        }

        freeaddrinfo(result);

        return 0;
    }

    i32 server::close_server()
    {
        closesocket(listen_socket);
        return 0;
    }

    i32 server::start_listening()
    {
        i32 i_result;
        while (1)
        {
            i_result = listen(listen_socket, SOMAXCONN);
            if (i_result == SOCKET_ERROR) {
                printf("listen failed with error: %d\n", WSAGetLastError());
                closesocket(listen_socket);
                return 1;
            }

            SOCKET ClientSocket = INVALID_SOCKET;
            ClientSocket = accept(listen_socket, NULL, NULL);
            if (ClientSocket == INVALID_SOCKET)
            {
                printf("accept failed with error: %d\n", WSAGetLastError());
                closesocket(listen_socket);
                return 1;

            }

            client_sockets.push_back(ClientSocket);
            DWORD   thread_id;
            HANDLE  thread_handle;

            thread_handle = CreateThread(
                NULL,                               // default security attributes
                0,                                  // use default stack size  
                launch_client_socket_static,        // thread function name
                (void*)this,                        // argument to thread function 
                0,                                  // use default creation flags   
                &thread_id);                        // returns the thread identifier 

            if (thread_handle == NULL)
            {
                ErrorHandler(TEXT("CreateThread"));
                ExitProcess(3);
            }
        }
        // No longer need server socket
    }

    DWORD WINAPI server::launch_client_socket_static(void* Param)
    {
        server* This = (server*)Param;
        return This->launch_client_socket();
    }

    DWORD server::launch_client_socket()
    {
        SOCKET client_socket = client_sockets.back();
        i32 i_result = 0;
        char recvbuf[DEFAULT_BUFLEN];

        // Receive until the peer shuts down the connection
        do {
            i_result = recv(client_socket, recvbuf, DEFAULT_BUFLEN, 0);
            if (i_result > 0)
            {
                printf("Bytes received: %d\n", i_result);

                // Echo the buffer back to the sender
                i32 iSendResult = 0;
                for (u32 i = 0; i < client_sockets.size(); ++i)
                {
                    iSendResult = send(client_socket, recvbuf, i_result, 0);
                    if (iSendResult == SOCKET_ERROR)
                    {
                        printf("send failed with error: %d\n", WSAGetLastError());
                        closesocket(client_socket);
                        WSACleanup();
                        return 1;
                    }
                    printf("Bytes sent: %d\n", iSendResult);
                }
            }
            else if (i_result == 0)
            {
                printf("Connection closing...\n");
            }
            else
            {
                printf("recv failed with error: %d\n", WSAGetLastError());
                closesocket(client_socket);

                WSACleanup();
                return 1;
            }
        } while (i_result > 0);

        // shutdown the connection since we're done
        i_result = shutdown(client_socket, SD_SEND);
        if (i_result == SOCKET_ERROR)
        {
            printf("shutdown failed with error: %d\n", WSAGetLastError());
            closesocket(client_socket);
            WSACleanup();
            return 1;
        }

        // cleanup
        closesocket(client_socket);
        return 0;
    }
}

int main(int argc, char* argv[])
{
    using namespace rain::server;

    printf("hello");

    init_winsock();
    //ip_ptr address = new ip();
    //address->address = "127.0.0.1:9998";

    server* rain_server = new server();
    rain_server->init("127.0.0.1", "5001");
    rain_server->start_listening();
    shutdown_winsock();
}