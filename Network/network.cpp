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
    if (c_client_socket != INVALID_SOCKET)
    {
        CloseSocket();
    }
    WSACleanup();
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
    std::cout << "Creating socket..." << std::endl;
    c_server_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (c_server_socket == INVALID_SOCKET) {
        std::cout << "ERROR: Socket creation failed! Error: " << WSAGetLastError() << std::endl;
    } else {
        std::cout << "Socket created successfully" << std::endl;
    }
}

void Network::BindSocket()
{
    std::cout << "Binding socket to 127.0.0.1:8080..." << std::endl;
    c_server_addr.sin_family = AF_INET;
    c_server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    c_server_addr.sin_port = htons(8080);

    int result = bind(c_server_socket, (sockaddr*)&c_server_addr, sizeof(c_server_addr));
    if (result == SOCKET_ERROR) {
        std::cout << "ERROR: Bind failed! Error: " << WSAGetLastError() << std::endl;
    } else {
        std::cout << "Bind successful" << std::endl;
    }
}

void Network::Listen()
{
    std::cout << "Listening for connections..." << std::endl;
    int result = listen(c_server_socket, SOMAXCONN);
    if (result == SOCKET_ERROR) {
        std::cout << "ERROR: Listen failed! Error: " << WSAGetLastError() << std::endl;
    } else {
        std::cout << "Server listening on 127.0.0.1:8080" << std::endl;
    }
}

void Network::AcceptClientConnection()
{
    std::cout << "Waiting for client connection..." << std::endl;
    c_client_socket = accept(c_server_socket, nullptr, nullptr);
    if (c_client_socket == INVALID_SOCKET) {
        std::cout << "ERROR: Accept failed! Error: " << WSAGetLastError() << std::endl;
    } else {
        std::cout << "Client connected!" << std::endl;
    }
}

void Network::DataFromClient()
{
    char buffer[1024] = {0};
    recv(c_client_socket, buffer, sizeof(buffer), 0);
    std::cout << "Message from client: " << buffer << std::endl;
}

void Network::CloseSocket()
{
    closesocket(c_server_socket);
    c_server_socket = INVALID_SOCKET;
}
