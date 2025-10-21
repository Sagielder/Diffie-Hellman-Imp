#include "Client.h"

void Client::CreateSocket()
{
    c_client_socket = socket(AF_INET, SOCK_STREAM, 0);
}

void Client::BindSocket()
{
    c_server_addr.sin_family = AF_INET;
    c_server_addr.sin_port = htons(8080);
    c_server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
}

void Client::SendConnectionRequest()
{
    connect(c_client_socket, (struct sockaddr*)&c_server_addr,
            sizeof(c_server_addr));
}

void Client::SendData()
{
    const char* message = "Hello, server!";
    send(c_client_socket, message, strlen(message), 0);
}

void Client::CloseSocket()
{
    close(c_client_socket);
    c_client_socket = -1;
}

Client::Client()
{
}

Client::~Client()
{
    if (c_client_socket != -1)
    {
        CloseSocket();
    }
}

void Client::Start()
{
    CreateSocket();
    BindSocket();
    SendConnectionRequest();
    SendData();
}

void Client::End()
{
    CloseSocket();
}
