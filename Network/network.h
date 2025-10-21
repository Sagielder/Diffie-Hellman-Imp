#ifndef NETWORK_H
#define NETWORK_H
#include <winsock2.h>
#include <ws2tcpip.h>
#include <cstring>
#include <iostream>
#include <stdio.h>
#include <unistd.h>

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
    SOCKET c_server_socket = NULL;
    sockaddr_in c_server_addr;
    int c_client_socket;
};



#endif