#ifndef NETWORKMANAGER_H
#define NETWORKMANAGER_H

#include <thread>
#include <mutex>
#include <glm/gtx/string_cast.hpp>

#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string/split.hpp>

#include "network/Server.h"
#include "network/Client.h"
#include "engine/tank.h"
#include "engine/GameObject.h"

class NetworkManager {
private:
    std::shared_ptr<std::string> localTankMsg;
    std::shared_ptr<std::string> remoteTankMsg;
    std::shared_ptr<Tank>  localTank;
    std::vector<std::shared_ptr<Tank>> networkTanks;
    std::shared_ptr <std::thread> serverThread;
    std::shared_ptr <std::thread> clientThread;
    std::shared_ptr <std::mutex> localTankMutex;
    std::shared_ptr <std::mutex> remoteTankMutex;
public:
    NetworkManager(std::shared_ptr<Tank>  localTank, std::vector<std::shared_ptr<Tank>> networkTanks);
    void startServer();
    void startClient(std::string ip);
    void synchronize();
};

#endif