#ifndef NETWORKMANAGER_H
#define NETWORKMANAGER_H

#include "network/Server.h"
#include "network/Client.h"
#include "engine/GameObject.h"

class NetworkManager {
private:
    std::shared_ptr<std::string> localTank;
    std::shared_ptr<std::string> remoteTank;
public:
    NetworkManager(std::vector<std::shared_ptr<GameObject>> networkTanks, std::vector<std::shared_ptr<GameObject>> obstacles);
    void startServer();
    void startClient(std::string ip);
};

#endif