#include "server.h"
#include "network.h"



#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "AdvApi32.lib")


#include "engine/core/context.h"

#define DEFAULT_BUFLEN 512


namespace rain::engine
{
    namespace network
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


        i32 start_server_thread(const char* ip_address)
        {
            DWORD   thread_id;
            HANDLE  thread_handle;

            ip_ptr address = new ip();
            address->address = "127.0.0.1:9998";

            thread_handle = CreateThread(
                NULL,                   // default security attributes
                0,                      // use default stack size  
                start_server,           // thread function name
                address,                // argument to thread function 
                0,                      // use default creation flags   
                &thread_id);      // returns the thread identifier 

            if (thread_handle == NULL)
            {
                ErrorHandler(TEXT("CreateThread"));
                ExitProcess(3);
            }

            //WaitForMultipleObjects(1, &hThreadArray, TRUE, INFINITE);
            //// Close all thread handles and free memory allocations.

            //CloseHandle(hThreadArray);
            return 0;
        }


        DWORD _stdcall start_server(LPVOID lpParam)
        {
            int iResult;

            SOCKET ListenSocket = INVALID_SOCKET;
            SOCKET ClientSocket = INVALID_SOCKET;

            struct addrinfo *result = NULL;
            struct addrinfo hints;

            int iSendResult;
            char recvbuf[DEFAULT_BUFLEN];
            int recvbuflen = DEFAULT_BUFLEN;

            ZeroMemory(&hints, sizeof(hints));
            hints.ai_family = AF_INET;
            hints.ai_socktype = SOCK_STREAM;
            hints.ai_protocol = IPPROTO_TCP;
            hints.ai_flags = AI_PASSIVE;

            ip_ptr ip_address_and_port = (ip_ptr)lpParam;
            std::string ip;
            std::string port;
            core::string::pair_split(ip_address_and_port->address, ":", ip, port);

            // Resolve the server address and port
            iResult = getaddrinfo(ip.c_str(), port.c_str(), &hints, &result);
            if (iResult != 0) {
                RAIN_LOG_NETWORK("getaddrinfo failed with error: %d\n", iResult);
                return 1;
            }

            // Create a SOCKET for connecting to server
            ListenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
            if (ListenSocket == INVALID_SOCKET) {
                RAIN_LOG_NETWORK("socket failed with error: %ld\n", WSAGetLastError());
                freeaddrinfo(result);
                return 1;
            }

            // Setup the TCP listening socket
            iResult = bind(ListenSocket, result->ai_addr, (int)result->ai_addrlen);
            if (iResult == SOCKET_ERROR) {
                RAIN_LOG_NETWORK("bind failed with error: %d\n", WSAGetLastError());
                freeaddrinfo(result);
                closesocket(ListenSocket);
                return 1;
            }

            freeaddrinfo(result);

            iResult = listen(ListenSocket, SOMAXCONN);
            if (iResult == SOCKET_ERROR) {
                RAIN_LOG_NETWORK("listen failed with error: %d\n", WSAGetLastError());
                closesocket(ListenSocket);
                return 1;
            }

            // Accept a client socket
            ClientSocket = accept(ListenSocket, NULL, NULL);
            if (ClientSocket == INVALID_SOCKET) {
                RAIN_LOG_NETWORK("accept failed with error: %d\n", WSAGetLastError());
                closesocket(ListenSocket);
                return 1;
            }

            // No longer need server socket
            closesocket(ListenSocket);

            // Receive until the peer shuts down the connection
            do {

                iResult = recv(ClientSocket, recvbuf, recvbuflen, 0);
                if (iResult > 0) {
                    RAIN_LOG_NETWORK("Bytes received: %d\n", iResult);

                    // Echo the buffer back to the sender
                    iSendResult = send(ClientSocket, recvbuf, iResult, 0);
                    if (iSendResult == SOCKET_ERROR) {
                        RAIN_LOG_NETWORK("send failed with error: %d\n", WSAGetLastError());
                        closesocket(ClientSocket);
                        WSACleanup();
                        return 1;
                    }
                    RAIN_LOG_NETWORK("Bytes sent: %d\n", iSendResult);
                }
                else if (iResult == 0)
                    RAIN_LOG_NETWORK("Connection closing...\n");
                else {
                    RAIN_LOG_NETWORK("recv failed with error: %d\n", WSAGetLastError());
                    closesocket(ClientSocket);
                    WSACleanup();
                    return 1;
                }

            } while (iResult > 0);

            // shutdown the connection since we're done
            iResult = shutdown(ClientSocket, SD_SEND);
            if (iResult == SOCKET_ERROR) {
                RAIN_LOG_NETWORK("shutdown failed with error: %d\n", WSAGetLastError());
                closesocket(ClientSocket);
                WSACleanup();
                return 1;
            }

            // cleanup
            closesocket(ClientSocket);
            return 0;
        }
    }
}