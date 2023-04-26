#ifndef OBJECTPOOL_H
#define OBJECTPOOL_H

#include <vector>
#include <memory>

#include "GameObject.h"

static class ObjectPool
{
public:
	ObjectPool();
	~ObjectPool();
	static void addGameObject(std::shared_ptr<GameObject> gameObject);
	static std::vector<std::shared_ptr<GameObject>> getGameObjects();

private:
	static std::vector<std::shared_ptr<GameObject>> gameObjects;
};

#endif // !OBJECTPOOL_H

