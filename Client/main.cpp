#include "Client.h"
#include <string>
#include <thread>

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
int main()
{
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
            std::cout << "Server disconnected." << std::endl;
            break;
        } else {
            std::cerr << "recv() failed: " << WSAGetLastError() << std::endl;
            break;
        }
    }

    
    return 0;
}