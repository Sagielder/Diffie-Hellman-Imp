#ifndef CLIENT_H
#define CLIENT_H
#include <winsock2.h>
#include <iostream>
#include <ws2tcpip.h>
class Client
{
private:
    void CreateSocket();
    void SetupServerAddress();
    void SendConnectionRequest();
    void SendData();
    void CloseSocket();
public:
    Client();
    ~Client();
    void Start();
    void End();
private:
    sockaddr_in c_server_addr;
    SOCKET c_client_socket = INVALID_SOCKET;
};





#endif