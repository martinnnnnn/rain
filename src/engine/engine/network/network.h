//#pragma once
//
//
//#include <winsock2.h>
//
//#if defined(_WIN32)
//#pragma comment( lib, "wsock32.lib" )
//#endif
//
//#include "core/core.h"
//#include "engine/core/context.h"
//
//namespace rain::engine
//{
//
//
//    namespace network
//    {
//
//        struct ip_address
//        {
//            union
//            {
//                u8 as_array[4];
//                u32 as_u32;
//            };
//        };
//
//        enum class PROTOCOL
//        {
//            TCP,
//            UDP
//        };
//
//        struct socket
//        {
//            
//
//            i32 handle;
//
//            ip_address address;
//            u16 port;
//
//            PROTOCOL protocol;
//            u64 non_blocking;
//        };
//
//        bool init()
//        {
//            ::WSADATA wsaData;
//            return ::WSAStartup(MAKEWORD(2, 2), &wsaData) == NO_ERROR;
//        }
//
//        void terminate()
//        {
//            WSACleanup();
//        }
//
//        i32 open_socket(socket& sock)
//        {
//            sock.handle = ::socket(AF_INET, SOCK_DGRAM, sock.protocol == PROTOCOL::UDP ? IPPROTO_UDP : IPPROTO_TCP);
//
//            if (sock.handle <= 0)
//            {
//                RAIN_LOG_NETWORK("failed to create socket\n");
//                return -1;
//            }
//
//            ::sockaddr_in address;
//            address.sin_family = AF_INET;
//            address.sin_addr.s_addr = sock.address.as_u32;
//            address.sin_port = htons(sock.port);
//
//            if (::bind(sock.handle, (const ::sockaddr*)&address, sizeof(::sockaddr_in)) < 0)
//            {
//                RAIN_LOG_NETWORK("failed to bind socket\n");
//                return false;
//            }
//
//            DWORD non_blocking = (sock.non_blocking) ? 1 : 0;
//            if (::ioctlsocket(sock.handle, FIONBIO, &non_blocking) != 0)
//            {
//                RAIN_LOG_NETWORK("failed to set blocking state\n");
//                return false;
//            }
//        }
//
//        void sent_packet(const socket& sock, const ip_address receiver_addr, const u16 receiver_port, const char* data, const u32 size)
//        {
//            ::sockaddr_in address;
//            address.sin_family = AF_INET;
//            address.sin_addr.s_addr = receiver_addr.as_u32;
//            address.sin_port = htons(receiver_port);
//
//            int size_sent = ::sendto(sock.handle, data, size, 0, (sockaddr*)&address, sizeof(sockaddr_in));
//
//            if (size_sent != size)
//            {
//                RAIN_LOG_NETWORK("failed to send packet\n");
//            }
//        }
//
//
//
//        //void start_listening_thread(socket s, )
//    }
//    
//    
//    
//    
//    // create listening socket
//    // create sending socket
//
//    
//
//
//
//    // init network
//    // init socket
//    // connect socket(ip, port, protocol)
//    // listen ()
//
//
//
//
//    namespace network
//    {
//        bool init_sockets()
//        {
//            ::WSADATA wsaData;
//            return ::WSAStartup(MAKEWORD(2, 2), &wsaData) == NO_ERROR;
//        }
//
//        void close_sockets()
//        {
//            WSACleanup();
//        }
//    };
//
//
//    bool create_socket(const u32 ipAdress, const u16 port)
//    {
//        int handle = ::socket(AF_INET,
//            SOCK_DGRAM,
//            IPPROTO_UDP);
//
//        if (handle <= 0)
//        {
//            RAIN_LOG("failed to create socket\n");
//            return false;
//        }
//
//        ::sockaddr_in address;
//        address.sin_family = AF_INET;
//        address.sin_addr.s_addr = ipAdress;
//        address.sin_port = htons(port);
//
//        if (::bind(handle, (const ::sockaddr*)&address, sizeof(::sockaddr_in)) < 0)
//        {
//            RAIN_LOG("failed to bind socket\n");
//            return false;
//        }
//
//        DWORD nonBlocking = 1;
//        if (::ioctlsocket(handle, FIONBIO, &nonBlocking) != 0)
//        {
//            RAIN_LOG("failed to set non-blocking\n");
//            return false;
//        }
//    }
//
//    bool send_packet(i32 _handle, const char* _data, sockaddr_in _adress, const u32 _size)
//    {
//        int sent_bytes = ::sendto(_handle, _data, _size, 0, (sockaddr*)&_adress, sizeof(sockaddr_in));
//
//        if (sent_bytes != _size)
//        {
//            RAIN_LOG("failed to send packet\n");
//            return false;
//        }
//    }
//
//    /*
//    * 
//    * launch listening thread
//    * 
//    *
//    */
//}