#ifndef TANK_H
#define	TANK_H
#include "./engine/GameObject.h"

class Tank : public GameObject
{
public:
	Tank(GLuint programID, std::string stlPath);
	~Tank();
	void render();
	void update();
private:
	GLuint vaoID;
	GLuint vbo[2];
	GLuint vb_size;
	void initializeBuffers();
	void cleanupBuffers();
	bool onCollissionEnter();
	std::string nickname;
	float speed;
	float reloadTime;
	int health;
};

#endif // !TANK_H
