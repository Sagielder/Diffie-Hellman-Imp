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
    for (size_t i = 0; i < c_client_list.size(); i++)
    {
        closesocket(c_client_list[i]->socket);
        free(c_client_list[i]);
    }
    WSACleanup();
}


void Network::Start()
{
    CreateSocket();
    BindSocket();
    Listen();
    
    std::cout << "Server is running. Press Ctrl+C to stop." << std::endl;
    
    // Keep accepting clients in a loop
    while (true)
    {
        FD_ZERO(&c_readfds);

        FD_SET(c_server_socket, &c_readfds);

        for (Client* client: c_client_list)
        {
            FD_SET(client->socket, &c_readfds);
        }

        size_t max_sd = c_server_socket;
        for (Client* client: c_client_list)
            if (client->socket > max_sd) max_sd = client->socket;

        int activity = select(max_sd + 1, &c_readfds, nullptr, nullptr, nullptr);

        if (activity < 0) {
            std::cout << "select() error: " << WSAGetLastError() << std::endl;
            break;
        }
        AcceptClientConnection();


        DataFromClient();
        
    }
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
    if (FD_ISSET(c_server_socket, &c_readfds))
    {
        SOCKET new_client_socket = accept(c_server_socket, nullptr, nullptr);
        if (new_client_socket != INVALID_SOCKET)
        {
            Client* newClient = new Client();
            newClient->socket = new_client_socket;
            c_client_list.push_back(newClient);
        }
    }
}

void Network::DataFromClient()
{
    for (size_t i = 0; i < c_client_list.size(); i++)
    {
        Client* client = c_client_list[i];
        if (FD_ISSET(client->socket, &c_readfds))
        {
            char buffer[1024];
            int bytes = recv(client->socket, buffer, sizeof(buffer), 0);
            if (bytes <= 0) {
                if (client->name == "")
                {
                    std::cout << "Client disconnected." << std::endl;
                }
                else
                {
                    std::cout << client->name << " disconnected." << std::endl;
                }
                closesocket(client->socket);
                c_client_list.erase(c_client_list.begin() + i);
                --i;
            } else {
                buffer[bytes] = '\0';
                if (client->name.empty())
                {
                    client->name = buffer;
                    std::cout << "Set Client Port (" << client->socket << ")'s name to " << client->name << std::endl;
                }
                else
                {
                    std::cout << "Received from " << client->name << " :" << buffer << std::endl;
                }
            }

        }
    }
}

void Network::CloseSocket()
{
    closesocket(c_server_socket);
    c_server_socket = INVALID_SOCKET;
}
