#ifndef NETWORK_H
#define NETWORK_H
#include <winsock2.h>
#include <ws2tcpip.h>
#include <cstring>
#include <iostream>
#include <stdio.h>

class Network
{
public:
    Network();
    ~Network();
    void Start();
    void End();
private:
    void CreateSocket();
    void BindSocket();
    void Listen();
    void AcceptClientConnection();
    void DataFromClient();
    void CloseSocket();

private:
    SOCKET c_server_socket = INVALID_SOCKET;
    sockaddr_in c_server_addr;
    SOCKET c_client_socket = INVALID_SOCKET;
};



#endif