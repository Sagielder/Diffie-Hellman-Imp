#ifndef NETWORK_H
#define NETWORK_H
#include <winsock2.h>
#include <ws2tcpip.h>
#include <cstring>
#include <iostream>
#include <stdio.h>
#include <cstdint>
#include <vector>

struct Client {
    SOCKET socket;
    std::string name = "";                        
};

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

    fd_set c_readfds;
    fd_set c_writefds;
    fd_set c_exceptfds;

    std::vector<Client*> c_client_list;
};



#endif