//#include "client.h"
//
//#include <glm/glm.hpp>
//
//namespace rain::engine
//{
//    void init_socket(Client* _client)
//    {
//        WSADATA wsaData;
//        WSAStartup(MAKEWORD(2, 2), &wsaData);
//
//        _client->serverInfoLenght = sizeof(_client->serverInfo);
//        _client->serverInfo.sin_family = AF_INET;
//        _client->serverInfo.sin_port = htons(1234);
//        _client->serverInfo.sin_addr.s_addr = inet_addr("127.0.0.1");
//
//        _client->socket = socket(AF_INET, SOCK_DGRAM, 0);
//    }
//
//    void close_socket(Client * _client)
//    {
//        closesocket(_client->socket);
//        ZeroMemory(_client, sizeof(_client));
//    }
//
//    void send_data(Client * _client, const char * _buffer, u32 _size)
//    {
//        if (sendto(_client->socket, _buffer, _size, 0, (sockaddr*)&_client->serverInfo, _client->serverInfoLenght) != SOCKET_ERROR)
//        {
//            //RAIN_LOG_NETWORK("sent package to server! %d", _size);
//        }
//    }
//
//    void check_receive_data(Client * _client, char * _buffer, u32 _size)
//    {
//        if (recvfrom(_client->socket, _buffer, _size, 0, (sockaddr*)&_client->serverInfo, &_client->serverInfoLenght) != SOCKET_ERROR)
//        {
//            RAIN_LOG_NETWORK("Receive response from server: %s", _buffer);
//        }
//    }
//
//
//    void* Packet::Serialize()
//    {
//        struct SerializedPacket *s = new SerializedPacket();
//        s->senderId = htonl(this->senderId);
//        s->sequenceNumber = htonl(this->sequenceNumber);
//        memcpy(s->data, this->data, MaxDataSize);
//        return s;
//    }
//
//    void Packet::Deserialize(char *message)
//    {
//        struct SerializedPacket *s = (struct SerializedPacket*)message;
//        this->senderId = ntohl(s->senderId);
//        this->sequenceNumber = ntohl(s->sequenceNumber);
//        memcpy(this->data, s->data, MaxDataSize);
//    }
//}