#ifndef GROUND_H
#define	GROUND_H
#include "./engine/GameObject.h"

class Ground : public GameObject
{
public:
	Ground(GLuint programID, std::string stlPath);
	~Ground();
	void render();
	void update();
private:
	GLuint vaoID;
	GLuint vb;
	GLuint vb_size;
	void initializeBuffers();
	void cleanupBuffers();
	bool onCollissionEnter();
	GLuint uvbuffer;
	GLuint textureSamplerID;
	GLuint texture;
	unsigned char* data;
	int width, height;
	int nrChannels = 4;
};

#endif 
