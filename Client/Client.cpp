#include "Client.h"

void Client::CreateSocket()
{
    Logger::debug("Creating client socket...");
    c_client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (c_client_socket == INVALID_SOCKET) {
        Logger::debug("ERROR: Socket creation failed! Error: " + WSAGetLastError());
    } else {
        Logger::debug("Socket created");
    }
}

void Client::SetupServerAddress()
{
    c_server_addr.sin_family = AF_INET;
    c_server_addr.sin_port = htons(8080);
    c_server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
}

void Client::SendConnectionRequest()
{
    Logger::info("Connecting to server...");
    int result = connect(c_client_socket, (struct sockaddr*)&c_server_addr,
            sizeof(c_server_addr));
    if (result == SOCKET_ERROR) {
        Logger::info("ERROR: Connect failed! Error: " + WSAGetLastError());
    } else {
        Logger::info("Connected to server!");
    }
}

void Client::SendData(const char* string_message)
{
    Logger::debug("Sending message...");
    int sent = send(c_client_socket, string_message, strlen(string_message), 0);
    if (sent == SOCKET_ERROR) {
        Logger::debug("ERROR: Send failed! Error: " + WSAGetLastError());
    } else {
        Logger::debug("Sent " + std::to_string(sent) + " bytes");
    }
}

void Client::CloseSocket()
{
    closesocket(c_client_socket);
    c_client_socket = INVALID_SOCKET;
}

Client::Client()
{
    WSADATA wsaData;
    WSAStartup(MAKEWORD(2,2), &wsaData);
}

Client::~Client()
{
    if (c_client_socket != INVALID_SOCKET)
    {
        CloseSocket();
    }
    WSACleanup();
}

void Client::Start()
{
    CreateSocket();
    SetupServerAddress();
    SendConnectionRequest();
}

void Client::End()
{
    CloseSocket();
}
