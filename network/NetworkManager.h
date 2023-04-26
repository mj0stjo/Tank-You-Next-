#ifndef NETWORKMANAGER_H
#define NETWORKMANAGER_H

#include "network/Server.h"
#include "network/Client.h"

class NetworkManager {
private:
    std::shared_ptr<std::string> localTank;
    std::shared_ptr<std::string> remoteTank;
public:
    NetworkManager();
    void startServer();
    void startClient(std::string ip);
};

#endif