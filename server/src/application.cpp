#include <winsock2.h>
#include <stdio.h>
#pragma comment (lib, "Ws2_32.lib")

void InitWinsock()
{
    WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);
}

struct Vec3
{
    float x;
    float y;
    float z;
};

struct Vec4
{
    float x;
    float y;
    float z;
    float w;
};

struct Transform 
{
    Vec3 position;
    Vec4 orientation;
    Vec3 scale;
};


#define PACK( __Declaration__ ) __pragma( pack(push, 1) ) __Declaration__ __pragma( pack(pop) )
const int MaxDataSize = 1082;

class Packet
{
public:
    int senderId;
    int sequenceNumber;
    char data[MaxDataSize];
public:
    void* Serialize();
    void Deserialize(char *message);
};

PACK(
    struct SerializedPacket {
    int senderId;
    int sequenceNumber;
    char data[MaxDataSize];
});

void* Packet::Serialize()
{
    struct SerializedPacket *s = new SerializedPacket();
    s->senderId = htonl(this->senderId);
    s->sequenceNumber = htonl(this->sequenceNumber);
    memcpy(s->data, this->data, MaxDataSize);
    return s;
}

void Packet::Deserialize(char *message)
{
    struct SerializedPacket *s = (struct SerializedPacket*)message;
    this->senderId = ntohl(s->senderId);
    this->sequenceNumber = ntohl(s->sequenceNumber);
    memcpy(this->data, s->data, MaxDataSize);
}

int main(int argc, char* argv[])
{
    SOCKET socketS;

    InitWinsock();
    struct sockaddr_in local;
    struct sockaddr_in from;
    int fromlen = sizeof(from);
    local.sin_family = AF_INET;
    local.sin_port = htons(1234);
    local.sin_addr.s_addr = INADDR_ANY;

    socketS = socket(AF_INET, SOCK_DGRAM, 0);
    bind(socketS, (sockaddr*)&local, sizeof(local));
    while (1)
    {
        char buffer[2048];
        ZeroMemory(buffer, sizeof(buffer));
        printf("Server : Waiting for paquet...\n");
        if (recvfrom(socketS, buffer, sizeof(buffer), 0, (sockaddr*)&from, &fromlen) != SOCKET_ERROR)
        {
            Packet p;
            p.Deserialize(buffer);


            printf("senderId : %d\nsequenceNumber : %d\ndata %s\n",
                p.senderId, p.sequenceNumber, p.data);

            //printf("Server : Received message from %s: %s\n", inet_ntoa(from.sin_addr), buffer);
            //sendto(socketS, buffer, sizeof(buffer), 0, (sockaddr*)&from, fromlen);
        }
        Sleep(500);
    }
    closesocket(socketS);

    return 0;
}