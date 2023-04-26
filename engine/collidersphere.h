#ifndef COLLIDERSPHERE_H
#define COLLIDERSPHERE_H	

#include <glm/glm.hpp>
#include <vector>
#include <memory>
#include "./engine/GameObject.h"

class GameObject;

class ColliderSphere
{
public:
	ColliderSphere(float radius);
	~ColliderSphere();
	bool checkCollision(std::shared_ptr<ColliderSphere> colliderSphere);
	float getRadius() const;
	void setCenter(glm::vec3 center);
	glm::vec3 getCenter() const;
private:
	float radius;
	glm::vec3 center;

};

#endif // !COLLIDERSPHERE_H
