#include "server.h"



#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "AdvApi32.lib")


#define DEFAULT_BUFLEN 512
#define DEFAULT_ADDRESS "127.0.0.1:5001"

namespace rain::server
{
    
    void print_error(i32 code, const char* fn)
    {
        LPVOID lpMsgBuf;

        FormatMessage(
            FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
            NULL,
            code,
            MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US),
            (LPTSTR)&lpMsgBuf,
            0, NULL);
        RAIN_LOG_SERVER("/!\\ %s failed with error %d: %s", fn, code, lpMsgBuf);
    }

    i32 init_winsock()
    {
        WSADATA wsa_data;
        i32 i_result = WSAStartup(MAKEWORD(2, 2), &wsa_data);
        if (i_result != 0)
        {
            print_error(i_result, "WSAStartup");
            return 1;
        }
        return 0;
    }

    i32 shutdown_winsock()
    {
        WSACleanup();
        return 0;
    }

    void print_fn(const char* str)
    {
        printf(str);
    }

    i32 server::init(const char* ip, const char* port)
    {
        RAIN_SERVER_CONTEXT->logger = new core::logger(print_fn);

        RAIN_LOG_SERVER("\n\n-----------------------------------------------\n\n");
        RAIN_LOG_SERVER("Initializing server @ %s:%s...", ip, port);
        int i_result;

        listen_socket = INVALID_SOCKET;

        struct addrinfo *result = NULL;
        struct addrinfo hints;

        ZeroMemory(&hints, sizeof(hints));
        hints.ai_family = AF_INET;
        hints.ai_socktype = SOCK_STREAM;
        hints.ai_protocol = IPPROTO_TCP;
        hints.ai_flags = AI_PASSIVE;

        RAIN_LOG_SERVER("   -> Resolving address and port...");
        i_result = ::getaddrinfo(ip, port, &hints, &result);
        if (i_result != 0)
        {
            print_error(i_result, "getaddrinfo");
            return 1;
        }
        RAIN_LOG_SERVER("   -> Done");

        RAIN_LOG_SERVER("   -> Creating socket...");
        listen_socket = ::socket(result->ai_family, result->ai_socktype, result->ai_protocol);
        if (listen_socket == INVALID_SOCKET)
        {
            print_error(WSAGetLastError(), "socket constructor");
            freeaddrinfo(result);
            return 1;
        }
        RAIN_LOG_SERVER("   -> Done");

        RAIN_LOG_SERVER("   -> Binding the socket to the address...");
        i_result = ::bind(listen_socket, result->ai_addr, (int)result->ai_addrlen);
        if (i_result == SOCKET_ERROR)
        {

            print_error(WSAGetLastError(), "bind");
            ::freeaddrinfo(result);
            ::closesocket(listen_socket);
            return 1;
        }
        RAIN_LOG_SERVER("   -> Done");
        RAIN_LOG_SERVER("Initialization done, freeing ressources...");
        ::freeaddrinfo(result);

        return 0;
    }

    i32 server::close_server()
    {
        closesocket(listen_socket);
        return 0;
    }

    i32 server::start_listening()
    {
        RAIN_LOG_SERVER("Starting to listen for clients...");

        i32 i_result;
        while (1)
        {
            RAIN_LOG_SERVER("   -> Listening...");
            i_result = listen(listen_socket, SOMAXCONN);
            if (i_result == SOCKET_ERROR)
            {
                print_error(WSAGetLastError(), "listen");
                closesocket(listen_socket);
                return 1;
            }
            SOCKET client_socket = INVALID_SOCKET;
            RAIN_LOG_SERVER("   -> Waiting for a connexion request...");
            client_socket = accept(listen_socket, NULL, NULL);
            if (client_socket == INVALID_SOCKET)
            {
                print_error(WSAGetLastError(), "accept");
                continue;
            }
            RAIN_LOG_SERVER("   -> New client connexion successful !\n");
            RAIN_LOG_SERVER("   -> Launching new thread to handle it\n");
            client_sockets.push_back(client_socket);
            DWORD   thread_id;
            HANDLE  thread_handle;
            thread_handle = CreateThread(NULL, 0, launch_client_socket_static, (void*)this, 0, &thread_id);

            if (thread_handle == NULL)
            {
                print_error(GetLastError(), "CreateThread");
                ExitProcess(3);
            }
        }
    }

    DWORD WINAPI server::launch_listening_static(void* Param)
    {
        server* This = (server*)Param;
        return This->start_listening();
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

        do {
            i_result = recv(client_socket, recvbuf, DEFAULT_BUFLEN, 0);
            if (i_result > 0)
            {
                printf("Bytes received: %d\n", i_result);
                printf("Text received : %s\n", recvbuf);

                // Echo the buffer back to the sender
                i32 iSendResult = 0;
                for (u32 i = 0; i < client_sockets.size(); ++i)
                {
                    if (client_sockets[i] != client_socket)
                    {
                        iSendResult = send(client_sockets[i], recvbuf, i_result, 0);
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
    enum class console_command
    {
        INIT_SERVER,
        SHUTDOWN_SERVER,
        START_LISTEN,
        GLOBAL_MESSAGE,
        LIST_CLIENT,
        KICK_CLIENT,
        HELP,
        QUIT,
        UNKNOWN
    };

    console_command parse_command(const char* str)
    {
        if (strcmp(str, "quit") == 0)
        {
            return console_command::QUIT;
        }
        if (strcmp(str, "help") == 0)
        {
            return console_command::HELP;
        }
        if (strcmp(str, "init") == 0)
        {
            return console_command::INIT_SERVER;
        }
        if (strcmp(str, "listen") == 0)
        {
            return console_command::START_LISTEN;
        }
        if (strcmp(str, "tell") == 0)
        {
            return console_command::GLOBAL_MESSAGE;
        }
        if (strcmp(str, "list") == 0)
        {
            return console_command::LIST_CLIENT;
        }
        if (strcmp(str, "kick") == 0)
        {
            return console_command::KICK_CLIENT;
        }
        if (strcmp(str, "shutdown") == 0)
        {
            return console_command::SHUTDOWN_SERVER;
        }
        return console_command::UNKNOWN;
    }
}

int main(int argc, char* argv[])
{
    using console_command = rain::server::console_command;
    
    rain::server::server* rain_server = new rain::server::server();
    rain::server::init_winsock();

    console_command command = console_command::UNKNOWN;
    std::vector<std::string> cmd_list;

    char buf[64];
    while (command != console_command::QUIT)
    {
        printf("cmd :>");
        fgets(buf, sizeof(buf), stdin);
        cmd_list = rain::core::string::split(rain::core::string::trim(buf), " ");

        if (cmd_list.size() == 0)
        {
            continue;
        }
        command = rain::server::parse_command(cmd_list[0].c_str());

        switch (command)
        {
        case console_command::HELP:
        {
            printf("quit                   -> shutdown server and quit application\n");
            printf("init <ip>:<port>       -> initialize server with the ip and port specified\n");
            printf("listen                 -> start listening - server needs to be initialized\n");
            printf("shutdown               -> shut the server down\n");
            printf("tell <msg>             -> send a message to all clients\n");
            printf("list                   -> get the list of all clients\n");
            printf("kick <client_name>     -> kick a client using his name\n");
            printf("\n");
            break;
        }
        case console_command::INIT_SERVER:
        {
            std::string ip;
            std::string port;
            if (cmd_list.size() == 2)
            {
                rain::core::string::pair_split(cmd_list[1], ":", ip, port);
            }
            else
            {
                printf("> No ip address and port specified, defaulting to 127.0.0.1:5001\n");
                rain::core::string::pair_split(DEFAULT_ADDRESS, ":", ip, port);
            }
            if (rain_server->init(ip.c_str(), port.c_str()) != 0)
            {
                printf("> Server coun't initialize... where did you fuck up ?\n");
            }
            break;
        }
        case console_command::START_LISTEN:
            DWORD   thread_id;
            HANDLE  thread_handle;
            thread_handle = CreateThread(NULL, 0, rain::server::server::launch_listening_static, (void*)rain_server, 0, &thread_id);

            if (thread_handle == NULL)
            {
                rain::server::print_error(GetLastError(), "CreateThread");
                ExitProcess(3);
            }
            break;
        case console_command::SHUTDOWN_SERVER:
            printf("not implemented yet\n");
            break;
        case console_command::GLOBAL_MESSAGE:
            printf("not implemented yet\n");
            break;
        case console_command::LIST_CLIENT:
            printf("not implemented yet\n");
            break;
        case console_command::KICK_CLIENT:
            printf("not implemented yet\n");
            break;
        case console_command::UNKNOWN:
            printf("Unknown command : type /help for informations on the various commands\n");
            break;
        }

    }
    
    // close connexions
    // shutdown server
    
    rain::server::shutdown_winsock();




    return 0;
}