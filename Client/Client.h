#ifndef CLIENT_H
#define CLIENT_H
#include <winsock2.h>
#include <ws2tcpip.h>
#include <unistd.h>
class Client
{
private:
    void CreateSocket();
    void BindSocket();
    void SendConnectionRequest();
    void SendData();
    void CloseSocket();
public:
    Client(/* args */);
    ~Client();
    void Start();
    void End();
private:
    sockaddr_in c_server_addr;
    int c_client_socket = -1;
};





#endif