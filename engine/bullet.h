#ifndef BULLET_H
#define BULLET_H
#include "engine/GameObject.h"

class Bullet : public GameObject
{
public:
	glm::vec3 direction;

	Bullet(float speed, int damage, glm::vec3 direction, glm::vec3 startPos, GLuint programID, std::string stlPath, std::string name);
	~Bullet();
	void update(float deltaTime);
	void render();
	bool onCollissionEnter(std::shared_ptr<GameObject> collissionObj);

private:
	void initializeBuffers();
	void cleanupBuffers();
	float speed;
	glm::vec3 direction;
	int damage;
	GLuint vaoID;
	GLuint vb;
	GLuint vb_size;
	float lifeTime;
	float upwardsForce;
};


#endif // !BULLET_H