#include "network/NetworkManager.h"

NetworkManager::NetworkManager() {
	localTank = std::make_shared<std::string>("clientTank");
	remoteTank = std::make_shared<std::string>("");
}

void NetworkManager::startServer() {
	Server s{ localTank, remoteTank };
	s.start();
}

void NetworkManager::startClient(std::string ip) {
	Client c{ localTank, remoteTank, ip};
	c.start();
}