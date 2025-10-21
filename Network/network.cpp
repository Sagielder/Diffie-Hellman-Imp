#include "network.h"


Network::Network()
{
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2,2), &wsaData) != 0) {
        // Handle error
    }
}

Network::~Network()
{
    if (c_client_socket != NULL)
    {
        CloseSocket();
    }
}

void Network::Start()
{
    CreateSocket();
    BindSocket();
    Listen();
    AcceptClientConnection();
    DataFromClient();
}

void Network::End()
{
    CloseSocket();
}

void Network::CreateSocket()
{
    SOCKET server_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (server_socket == INVALID_SOCKET) {
        // Handle error with WSAGetLastError()
    }
}

void Network::BindSocket()
{
    c_server_addr.sin_family = AF_INET;
    c_server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    c_server_addr.sin_port = htons(8080);

    bind(c_server_socket, (sockaddr*)&c_server_addr, sizeof(c_server_addr));
}

void Network::Listen()
{
    listen(c_server_socket, SOMAXCONN);
}

void Network::AcceptClientConnection()
{
    c_client_socket = accept(c_server_socket, nullptr, nullptr);
}

void Network::DataFromClient()
{
    char buffer[1024] = {0};
    recv(c_client_socket, buffer, sizeof(buffer), 0);
    std::cout << "Message from client: " << buffer << std::endl;
}

void Network::CloseSocket()
{
    close(c_server_socket);
    c_server_socket = NULL;
}
