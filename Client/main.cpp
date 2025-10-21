#include "Client.h"
#include <string>

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

int main()
{
    Client client;
    std::string inputString;
    std::string client_name;
    std::cout << "Enter Your Username" << std::endl;
    std::cin >> client_name;
    client.Start();
    client.SendData(client_name.c_str());
    client_name.clear();
    client_name.shrink_to_fit();
    while (true)
    {
        std::cout << "Enter Your message, enter 'Ctrl+C' to end session" << std::endl;
        std::cin >> inputString;
        client.SendData(inputString.c_str());
        inputString.clear();
    }
    return 0;
}