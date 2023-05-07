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
	//Update position of local tank
	std::string localStr = std::to_string(localTank->getPosition().x) + " " + std::to_string(localTank->getPosition().y) + " " + std::to_string(localTank->getPosition().z) 
		+ " " + std::to_string(localTank->getRotation().x) + " " + std::to_string(localTank->getRotation().y) + " " + std::to_string(localTank->getRotation().z)
		+ " " + std::to_string(localTank->getKupelRotation().x) + " " + std::to_string(localTank->getKupelRotation().y) + " " + std::to_string(localTank->getKupelRotation().z);
	{
		std::lock_guard<std::mutex> lg(*localTankMutex);
		*localTankMsg = localStr;
	}
	//Update position of remote tank
	std::string remoteStr;
	{
		std::lock_guard<std::mutex> lg(*remoteTankMutex);
		remoteStr = *remoteTankMsg;
	}

	if (!remoteStr.empty()) {
		std::vector<std::string> vec;
		boost::split(vec, remoteStr, boost::is_any_of(" "), boost::token_compress_on);

		networkTanks[0]->setPosition(glm::vec3(std::stof(vec[0]), std::stof(vec[1]), std::stof(vec[2])));
		networkTanks[0]->setRotation(glm::vec3(std::stof(vec[3]), std::stof(vec[4]), std::stof(vec[5])));
		networkTanks[0]->setKupelRotation(glm::vec3(std::stof(vec[6]), std::stof(vec[7]), std::stof(vec[8])));
	}
}