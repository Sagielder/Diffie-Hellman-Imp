#include "network.h"
#include <sstream>


Command parseCommand(const char* buffer, int length)
{
    Command cmd;
    
    std::string data(buffer, length);
    std::istringstream iss(data);
    
    std::string firstWord;
    iss >> firstWord;
    
    if (firstWord == "/send")
    {
        cmd.commandType = CommandType::SEND;
        iss >> cmd.arg1;  // recipient
        iss >> cmd.arg2; // payload
        if (cmd.arg1.empty() || cmd.arg2.empty()) // cant be empty (for now)
        {
            cmd.commandType = CommandType::INVALID;
        }
    }
    else if (firstWord == "/help")
    {
        cmd.commandType = CommandType::HELP;
    }
    
    return cmd;
}


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
    
    Logger::info("Server is running. Press Ctrl+C to stop.");
    
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
            Logger::debug("select() error: " + WSAGetLastError());
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
    Logger::debug("Creating socket...");
    c_server_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (c_server_socket == INVALID_SOCKET) {
        Logger::debug("ERROR: Socket creation failed! Error: " + WSAGetLastError());
    } else {
        Logger::debug("Socket created successfully");
    }


}

void Network::BindSocket()
{
    Logger::debug("Binding socket to 127.0.0.1:8080...");
    c_server_addr.sin_family = AF_INET;
    c_server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    c_server_addr.sin_port = htons(8080);

    int result = bind(c_server_socket, (sockaddr*)&c_server_addr, sizeof(c_server_addr));
    if (result == SOCKET_ERROR) {
        Logger::debug("ERROR: Bind failed! Error: " + WSAGetLastError());
    } else {
        Logger::debug("Bind successful");
    }
}

void Network::Listen()
{
    Logger::debug("Listening for connections...");
    int result = listen(c_server_socket, SOMAXCONN);
    if (result == SOCKET_ERROR) {
        Logger::debug("ERROR: Listen failed! Error: " + WSAGetLastError());
    } else {
        Logger::debug("Server listening on 127.0.0.1:8080");
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

void Network::GetStringListUserInNetwork(std::string& strRet)
{
    strRet = "Current Users in Network"; 
    for (Client* client : c_client_list)
    {
        strRet += "\n- " + client->name;
    }

}

void Network::HandleHelpCommand()
{

}
void Network::DataFromClient()
{
    for (size_t i = 0; i < c_client_list.size(); i++)
    {
        Client* client = c_client_list[i];
        if (FD_ISSET(client->socket, &c_readfds))
        {
            char buffer[1024];
            int bytesReceived = recv(client->socket, buffer, sizeof(buffer), 0);
            if (bytesReceived <= 0) {
                if (client->name == "")
                {
                    Logger::debug("Client disconnected.");
                }
                else
                {
                    Logger::debug(client->name + " disconnected.");
                }
                closesocket(client->socket);
                map_name_client.erase(client->name);
                c_client_list.erase(c_client_list.begin() + i);
                --i;
            } else {
                buffer[bytesReceived] = '\0';
                if (client->name.empty())
                {
                    client->name = buffer;
                    Logger::debug("Set Client Port (" + std::to_string(client->socket) + ")'s name to " + client->name);
                    map_name_client.insert({client->name, client});
                    std::string string_user_list;
                    GetStringListUserInNetwork(string_user_list);
                    DataToClient(client, string_user_list.c_str());
                }
                else
                {
                    Logger::debug("Received from " + client->name + " :" + buffer);
                    
                    Command command;
                    command = parseCommand(buffer, bytesReceived);

                    if (command.commandType == CommandType::INVALID)
                    {
                        DataToClient(client, "Invalid Commnad");
                    }
                    else if (command.commandType == CommandType::SEND)
                    {
                        std::unordered_map<std::string, Client*>::iterator mapItor;
                        mapItor = map_name_client.find(command.arg1);
                        if (mapItor == map_name_client.end())
                        {
                            DataToClient(client, "User not found");
                        }
                        else
                        {
                            Client* recipient = mapItor->second;
                            DataToClient(recipient, ("[From "+ client->name +"]: "+ command.arg2).c_str());
                        }
                    }
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


void Network::DataToClient(Client* client, const char* string_message_content)
{
    Logger::debug("Sending message...");
    int sent = send(client->socket, string_message_content, strlen(string_message_content), 0);
    if (sent == SOCKET_ERROR) {
        Logger::debug("ERROR: Send failed! Error: " + WSAGetLastError());
    } else {
        Logger::debug("Sent " + std::to_string(sent) + " bytes");
    }
}