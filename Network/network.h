#ifndef NETWORK_H
#define NETWORK_H
#include <winsock2.h>
#include <ws2tcpip.h>
#include <cstring>
#include <iostream>
#include <stdio.h>
#include <cstdint>
#include <vector>
#include <unordered_map>
#include <logger.h>
#include <string>
#include <sstream>

enum class CommandType {
    INVALID, SEND, HELP
};

struct Command
{
    CommandType commandType = CommandType::INVALID;
    std::string arg1;     
    std::string arg2;
};



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
    void GetStringListUserInNetwork(std::string& strRet);
    void HandleHelpCommand();
    void DataFromClient();
    void CloseSocket();
    void DataToClient(Client* client, const char* string_message_content);

private:
    SOCKET c_server_socket = INVALID_SOCKET;
    sockaddr_in c_server_addr;

    fd_set c_readfds;
    fd_set c_writefds;
    fd_set c_exceptfds;
    std::unordered_map<std::string, Client*> map_name_client;
    std::vector<Client*> c_client_list;
};



#endif