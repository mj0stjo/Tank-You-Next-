#include "collidersphere.h"
#include "./engine/GameObject.h"

ColliderSphere::ColliderSphere(float radius)
{
	this->radius = radius;
	
}

ColliderSphere::~ColliderSphere()
{
}

glm::vec3 ColliderSphere::getCenter() const{
	return center;
}

void ColliderSphere::setCenter(glm::vec3 center) {
	this->center = center;
}

float ColliderSphere::getRadius() const {
	return radius;
}

bool ColliderSphere::checkCollision(std::shared_ptr<ColliderSphere> colliderSphere) {
	
	
	glm::vec3 otherCenter = colliderSphere->getCenter();
	
	float distance = glm::distance(center, otherCenter);
	
	return (distance < radius + colliderSphere->getRadius());
	
}

