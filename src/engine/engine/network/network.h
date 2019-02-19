#pragma once


#include "core/core.h"
#include "engine/win32/win32_helpers.h"


#define DEFAULT_BUFLEN 512

namespace rain::engine
{
    namespace network
    {
        enum class error
        {
            sending_failed,
            connexion_failed
        };


        enum class packet_type
        {
            world_update,
            textual_chat,
            vocal_chat
        };

        struct connexion_info
        {
            SOCKET socket;
        };

        struct INGAME_CHAT_INC
        {
            std::string msg;
        };


        i32 init();
        void terminate();
        int start_tcp_client(connexion_info& info, const char* ip_address, const char* port);
        i32 stop_tcp_client(const connexion_info& info);
        i32 send_packet(const connexion_info& info, const char* buffer);
    }
}