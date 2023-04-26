#ifndef GROUND_H
#define	GROUND_H
#include "./engine/GameObject.h"

class Ground : public GameObject
{
public:
	Ground(GLuint programID, std::string stlPath);
	~Ground();
	void render();
	void update(float deltaTime);
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

#endif 
