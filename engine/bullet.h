#ifndef BULLET_H
#define BULLET_H
#include "engine/GameObject.h"

class Bullet : public GameObject
{
public:
	Bullet(float speed, int damage, glm::vec3 direction, GLuint programID, std::string stlPath);
	~Bullet();
	void update();
	void render();
	bool onCollissionEnter();

private:
	void initializeBuffers();
	void cleanupBuffers();
	float speed;
	glm::vec3 direction;
	int damage;
	GLuint vaoID;
	GLuint vb;
	GLuint vb_size;
};


#endif // !BULLET_H