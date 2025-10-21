#include "Client.h"

void Client::CreateSocket()
{
    std::cout << "Creating client socket..." << std::endl;
    c_client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (c_client_socket == INVALID_SOCKET) {
        std::cout << "ERROR: Socket creation failed! Error: " << WSAGetLastError() << std::endl;
    } else {
        std::cout << "Socket created" << std::endl;
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
    std::cout << "Connecting to server..." << std::endl;
    int result = connect(c_client_socket, (struct sockaddr*)&c_server_addr,
            sizeof(c_server_addr));
    if (result == SOCKET_ERROR) {
        std::cout << "ERROR: Connect failed! Error: " << WSAGetLastError() << std::endl;
    } else {
        std::cout << "Connected to server!" << std::endl;
    }
}

void Client::SendData(const char* string_message)
{
    std::cout << "Sending message..." << std::endl;
    int sent = send(c_client_socket, string_message, strlen(string_message), 0);
    if (sent == SOCKET_ERROR) {
        std::cout << "ERROR: Send failed! Error: " << WSAGetLastError() << std::endl;
    } else {
        std::cout << "Sent " << sent << " bytes" << std::endl;
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
