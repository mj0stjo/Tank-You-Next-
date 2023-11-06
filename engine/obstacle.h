#ifndef OBSTACLE_H
#define	OBSTACLE_H
#include "./engine/GameObject.h"

class Obstacle : public GameObject
{
public:
	Obstacle(GLuint programID, std::string stlPath);
	~Obstacle();
	void update(float deltaTime);
	void render();
	bool onCollissionEnter(std::shared_ptr<GameObject> collissionObj);
	
private:
	GLuint vaoID;
	GLuint vb;
	GLuint vb_size;
	void initializeBuffers();
	void cleanupBuffers();
	GLuint uvbuffer;
	GLuint textureSamplerID;
	GLuint texture;
	unsigned char* data;
	int width, height;
	int nrChannels = 4;
};

#endif // !TANK_H
