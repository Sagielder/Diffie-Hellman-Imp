#include "network.h"

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
    Network myInsecureNetwork;

    myInsecureNetwork.Start();
    myInsecureNetwork.End();
    

    return 0;
};