#include "network/NetworkManager.h"

NetworkManager::NetworkManager(std::shared_ptr<Tank>  localTank, std::vector<std::shared_ptr<Tank>> networkTanks) {
	this->localTank = localTank;
	this->networkTanks = networkTanks;
	localTankMutex = std::make_shared<std::mutex>();
	remoteTankMutex = std::make_shared<std::mutex>();
	localTankMsg = std::make_shared<std::string>();
	remoteTankMsg = std::make_shared<std::string>();
	std::cout << "Initialized NetworkManager" << std::endl;
}

void NetworkManager::startServer() {
	try {
		serverThread = std::make_shared<std::thread>([&] {
			Server s{ localTankMsg, remoteTankMsg, remoteTankMutex, localTankMutex };
			});
	}
	catch(const std::exception& e){
		std::cout << "Creating server thread failed!" << std::endl;
	}
}

void NetworkManager::startClient(std::string ip) {
	try {
		clientThread = std::make_shared<std::thread>([&, ip]{
			Client c{ localTankMsg, remoteTankMsg, ip, remoteTankMutex, localTankMutex };
			});
	}
	catch (const std::exception& e) {
		std::cout << "Creating client thread failed!" << std::endl;
	}
}

void NetworkManager::synchronize() {
	{
		std::lock_guard<std::mutex> lg(*localTankMutex);
		*localTankMsg = glm::to_string(localTank->getPosition());;
	}
}