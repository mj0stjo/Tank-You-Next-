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

void NetworkManager::startServer(int maxClient) {
	try {
		serverThread = std::make_shared<std::thread>([&] {
			 boost::asio::io_service io_service;
			 Server s{ maxClient, localTankMsg, remoteTankMsg, remoteTankMutex, localTankMutex, io_service };
			 io_service.run();
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
		+ " " + std::to_string(localTank->getKupelRotation().x) + " " + std::to_string(localTank->getKupelRotation().y) + " " + std::to_string(localTank->getKupelRotation().z)
		+ " " + std::to_string(localTank->getColor().x) + " " + std::to_string(localTank->getColor().y) + " " + std::to_string(localTank->getColor().z);

	// get all local bullets
	std::vector<std::shared_ptr<Bullet>> localBullets = ObjectPool::getLocalBullets();

	for (int i = 0; i < localBullets.size(); i++) {

		std::string nameid = localBullets[i]->getName();
		// make substring of nameid cut out "localBullet"
		nameid = nameid.substr(11, nameid.length() - 1);

	

		localStr += "B" + std::to_string(localBullets[i]->position.x) + " " + std::to_string(localBullets[i]->position.y) + " " + std::to_string(localBullets[i]->position.z) + " " +
			std::to_string(localBullets[i]->direction.x) + " " + std::to_string(localBullets[i]->direction.y) + " " + std::to_string(localBullets[i]->direction.z) + " " +nameid;
	}


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
		std::vector<std::string> tanks;
		boost::split(tanks, remoteStr, boost::is_any_of("X"), boost::token_compress_on);

		for (int i = 0; i < tanks.size(); i++) {
			if (tanks[i].empty()) continue;

			std::vector<std::string> posAndBul;
			boost::split(posAndBul, tanks[i], boost::is_any_of("B"), boost::token_compress_on);

			std::vector<std::string> vec;
			boost::split(vec, posAndBul[0], boost::is_any_of(" "), boost::token_compress_on);

			networkTanks[i]->setPosition(glm::vec3(std::stof(vec[0]), std::stof(vec[1]), std::stof(vec[2])));
			networkTanks[i]->setRotation(glm::vec3(std::stof(vec[3]), std::stof(vec[4]), std::stof(vec[5])));
			networkTanks[i]->setKupelRotation(glm::vec3(std::stof(vec[6]), std::stof(vec[7]), std::stof(vec[8])));

			glm::vec3 color = glm::vec3(
				std::stof(vec[9]),
				std::stof(vec[10]),
				std::stof(vec[11])
			);

			networkTanks[i]->setColor(color);

			if (posAndBul.size() > 1) {

				for (int j = 1; j < posAndBul.size(); j++) {
						
					std::vector<std::string> bul;
					boost::split(bul, posAndBul[j], boost::is_any_of(" "), boost::token_compress_on);

					float bulletSpeed = 60.0f;
					int bulletDamage = 10;
					

					// id is in bul[6]
					std::string bulletName = "netBullet" + std::to_string(i) + "X" + bul[6];
					

					// if bullet is already in object pool, update it
					if (!ObjectPool::contains(bulletName)) {
						GLuint bulletShaderID = LoadShaders("../engine/BulletVShader.vertexshader", "../engine/BulletFShader.fragmentshader");
						std::shared_ptr<Bullet> bullet = std::make_shared<Bullet>(bulletSpeed, bulletDamage, glm::vec3(std::stof(bul[3]), std::stof(bul[4]), std::stof(bul[5])), glm::vec3(std::stof(bul[0]), std::stof(bul[1]), std::stof(bul[2])), bulletShaderID, "../models/monke.stl", bulletName);
						bullet->setName(bulletName);

						ObjectPool::addGameObject(bullet);
					}
					else {
						std::shared_ptr<Bullet> bulletToUpdate = ObjectPool::getBullet(bulletName);

						bulletToUpdate->direction = glm::vec3(std::stof(bul[3]), std::stof(bul[4]), std::stof(bul[5]));
						bulletToUpdate->position = glm::vec3(std::stof(bul[0]), std::stof(bul[1]), std::stof(bul[2]));
						
						
					}

					//std::cout << "Net-Bullet " << bulletName << " created" << std::endl;
					
				}
				
			}

		}

		
	}
}