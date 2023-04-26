#include "objectpool.h"

std::vector<std::shared_ptr<GameObject>> ObjectPool::gameObjects;

void ObjectPool::addGameObject(std::shared_ptr<GameObject> gameObject)
{
	gameObjects.push_back(gameObject);
}

std::vector<std::shared_ptr<GameObject>> ObjectPool::getGameObjects()
{
	// iterate through gameObjects and remove any that are destroyed
	for (int i = 0; i < gameObjects.size(); i++) {
		if (gameObjects.at(i)->isDestroyed()) {
			gameObjects.erase(gameObjects.begin() + i);
		}
	}
	
	return gameObjects;
}