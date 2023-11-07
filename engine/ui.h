#ifndef UI_H
#define	UI_H
#include "./engine/GameObject.h"

class Ui : public GameObject
{
public:
	Ui(GLuint programID, std::string stlPath);
	~Ui();
	void render();
	void update(float deltaTime);
	bool onCollissionEnter(std::shared_ptr<GameObject> collissionObj);
	void setRotation(float rot);
	void setPosition(float x, float z);
private:
	GLuint vaoID;
	GLuint vb;
	GLuint vb_size;
	void initializeBuffers();
	void cleanupBuffers();
};

#endif 
