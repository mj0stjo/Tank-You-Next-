#include "objectpool.h"

std::vector<std::shared_ptr<GameObject>> ObjectPool::gameObjects;

void ObjectPool::addGameObject(std::shared_ptr<GameObject> gameObject)
{
	gameObjects.push_back(gameObject);
}

std::vector<std::shared_ptr<GameObject>> ObjectPool::getGameObjects()
{
	return gameObjects;
}