//#include "network.h"
//
//#include <cassert>
//
//namespace rain
//{
//
//    Address::Address(u8 _a, u8 _b, u8 _c, u8 _d, u16 _port) : port(_port)
//    {
//        address.data[0] = _a;
//        address.data[1] = _b;
//        address.data[2] = _c;
//        address.data[3] = _d;
//
//        assert((address.full) == ((_a << 24) | (_b << 16) | (_b << 8) | _d));
//    }
//
//    inline Address::Address(u32 _address, u16 _port)
//        : port(_port)
//    {
//        address.full = _address;
//    }
//
//
//    Socket::Socket()
//        : handle(-1)
//    {
//        
//    }
//
//    Socket::~Socket()
//    {
//        if (handle != -1)
//        {
//            close();
//        }
//    }
//
//    bool Socket::bind(u16 port)
//    {
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
//    void Socket::close()
//    {
//        closesocket(handle);
//        handle = -1;
//    }
//
//    bool Socket::is_open() const
//    {
//
//    }
//
//    bool Socket::send(const Address & destination, const void * data, u32 size)
//    {
//
//    }
//
//    i32 Socket::receive(Address & sender, void* data, u32 size)
//    {
//
//    }
//
//}