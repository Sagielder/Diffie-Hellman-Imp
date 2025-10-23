#include "Client.h"
#include <string>
#include <thread>
#include <logger.h>
bool InsensitiveStringCompare(std::string str1, std::string str2)
{
    if (str1.length() != str2.length()) {
        return false;
    }
    for (size_t i = 0; i < str1.length(); ++i) {
        if (::tolower(str1[i]) != ::tolower(str2[i])) {
            return false;
        }
    }
    return true;
}

void inputLoopThread(Client* client)
{
    std::string inputString;
    while (true)
    {
        std::cout << "Enter Your message, enter 'Ctrl+C' to end session" << std::endl;
        std::getline(std::cin, inputString);
        client->SendData(inputString.c_str());
        inputString.clear();
    }
}
int main(int argc, char* argv[])
{
    bool debugMode = false;
    
    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "--debug" || arg == "-d") {
            debugMode = true;
        }
    }

    Logger::setDebugMode(debugMode);
    Client client;
    std::string client_name;
    std::cout << "Enter Your Username" << std::endl;
    std::getline(std::cin, client_name);
    client.Start();
    client.SendData(client_name.c_str());
    client_name.clear();
    client_name.shrink_to_fit();

    std::thread inputWorkerThread(inputLoopThread, &client);
    inputWorkerThread.detach();

    char buffer[1024];
    while (true) {
        int bytes = recv(client.c_client_socket, buffer, sizeof(buffer) - 1, 0);
        if (bytes > 0) {
            buffer[bytes] = '\0';
            std::cout << "[Server] " << buffer << std::endl;
        } else if (bytes == 0) {
            Logger::info("Server disconnected.");
            break;
        } else {
            Logger::debug("recv() failed: " + WSAGetLastError());
            break;
        }
    }

    
    return 0;
}