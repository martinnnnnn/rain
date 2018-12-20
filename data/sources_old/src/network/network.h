//#pragma once
//
//
//#include <winsock2.h>
//
//#if defined(_WIN32)
//#pragma comment( lib, "wsock32.lib" )
//#endif
//
//#include "core/logger.h"
//#include "core/types.h"
//
//namespace rain
//{
//    namespace network
//    {
//        bool init_sockets()
//        {
//            WSADATA wsaData;
//            return WSAStartup(MAKEWORD(2, 2), &wsaData) == NO_ERROR;
//        }
//
//        void close_sockets()
//        {
//            WSACleanup();
//        }
//    };
//
// /*   struct Address
//    {
//        Address() : Address(0, 0) {}
//        Address(u8 _a, u8 _b, u8 _c, u8 _d, u16 _port);
//        Address(u32 _address, u16 _port);
//
//        union
//        {
//            u32 full;
//            u8 data[4];
//        } address;
//
//        u16 port;
//    };
//
//    class Socket
//    {
//    public:
//
//        Socket();
//        ~Socket();
//
//        bool bind(u16 port);
//        void close();
//        bool is_open() const;
//        bool send(const Address & destination, const void * data, u32 size);
//        i32 receive(Address & sender, void* data, u32 size);
//
//    private:
//        i32 handle;
//    };
//*/
//
//
//    struct Sockett
//    {
//        i32 handle;
//    };
//
//    bool create_socket(const u32 ipAdress, const u16 port)
//    {
//        int handle = socket(AF_INET,
//            SOCK_DGRAM,
//            IPPROTO_UDP);
//
//        if (handle <= 0)
//        {
//            RAIN_LOG("failed to create socket\n");
//            return false;
//        }
//
//        sockaddr_in address;
//        address.sin_family = AF_INET;
//        address.sin_addr.s_addr = ipAdress;
//        address.sin_port = htons(port);
//
//        if (bind(handle, (const sockaddr*)&address, sizeof(sockaddr_in)) < 0)
//        {
//            RAIN_LOG("failed to bind socket\n");
//            return false;
//        }
//
//        DWORD nonBlocking = 1;
//        if (ioctlsocket(handle, FIONBIO, &nonBlocking) != 0)
//        {
//            RAIN_LOG("failed to set non-blocking\n");
//            return false;
//        }
//    }
//
//    bool send_packet(i32 _handle, const char* _data, sockaddr_in _adress, const u32 _size)
//    {
//        int sent_bytes = sendto(_handle, _data, _size, 0, (sockaddr*)&_adress, sizeof(sockaddr_in));
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