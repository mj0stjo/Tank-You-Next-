#include "ObjectPool.h"

std::unordered_map<std::string, std::shared_ptr<GameObject>> ObjectPool::gameObjectsMap;

void ObjectPool::addGameObject(const std::shared_ptr<GameObject>& gameObject)
{
	// Use the gameObject's name as the key for quick lookup
	gameObjectsMap[gameObject->getName()] = gameObject;
}

std::vector<std::shared_ptr<GameObject>> ObjectPool::getGameObjects()
{
    std::vector<std::shared_ptr<GameObject>> activeGameObjects;
    std::vector<std::string> destroyedObjectsKeys;

    // First pass: collect active objects and keys of destroyed objects
    for (auto& pair : gameObjectsMap) {
        if (!pair.second->isDestroyed()) {
            activeGameObjects.push_back(pair.second);
        }
        else {
            destroyedObjectsKeys.push_back(pair.first);
        }
    }

    // Second pass: remove destroyed objects from the map
    for (const auto& key : destroyedObjectsKeys) {
        gameObjectsMap.erase(key);
    }

    return activeGameObjects;
}

std::shared_ptr<Bullet> ObjectPool::getBullet(std::string name) {

    return std::dynamic_pointer_cast<Bullet>(gameObjectsMap[name]);
}


bool ObjectPool::contains(const std::string& name)
{
	auto it = gameObjectsMap.find(name);
	return it != gameObjectsMap.end() && !it->second->isDestroyed();
}

// method returning a list with all the bullets that contain "local" in their name
std::vector<std::shared_ptr<Bullet>> ObjectPool::getLocalBullets() {
    std::vector<std::shared_ptr<Bullet>> localBullets;

    for (const auto& pair : gameObjectsMap) {
        // Check if the name contains "local"
        if (pair.first.find("local") != std::string::npos) {

            // cast to Bullet
            std::shared_ptr<Bullet> bullet = std::dynamic_pointer_cast<Bullet>(pair.second);

            localBullets.push_back(bullet);
        }
    }

    return localBullets;
}

