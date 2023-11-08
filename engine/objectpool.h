#ifndef OBJECTPOOL_H
#define OBJECTPOOL_H

#include <memory>
#include <unordered_map>

#include "GameObject.h"
#include "bullet.h"

class ObjectPool
{
public:
	ObjectPool() = default;
	~ObjectPool() = default;

	static void addGameObject(const std::shared_ptr<GameObject>& gameObject);
	static std::vector<std::shared_ptr<GameObject>> getGameObjects();
	static bool contains(const std::string& name);
	static std::vector<std::shared_ptr<Bullet>> getLocalBullets();
	static std::shared_ptr<Bullet> getBullet(std::string name);

private:
	static std::unordered_map<std::string, std::shared_ptr<GameObject>> gameObjectsMap;
};

#endif // !OBJECTPOOL_H