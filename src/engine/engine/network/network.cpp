#include "network.h"

#include <cassert>

namespace rain::engine
{

 


    Socket::Socket()
        : handle(-1)
    {

    }

    Socket::~Socket()
    {
        if (handle != -1)
        {
            close();
        }
    }

    bool Socket::bind(u16 port)
    {
        if (bind(handle, (const sockaddr*)&address, sizeof(sockaddr_in)) < 0)
        {
            RAIN_LOG("failed to bind socket\n");
            return false;
        }

        DWORD nonBlocking = 1;
        if (ioctlsocket(handle, FIONBIO, &nonBlocking) != 0)
        {
            RAIN_LOG("failed to set non-blocking\n");
            return false;
        }
    }

    void Socket::close()
    {
        closesocket(handle);
        handle = -1;
    }

    bool Socket::is_open() const
    {

    }

    bool Socket::send(const Address & destination, const void * data, u32 size)
    {

    }

    i32 Socket::receive(Address & sender, void* data, u32 size)
    {

    }

}