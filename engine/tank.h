#ifndef TANK_H
#define	TANK_H
#include "./engine/GameObject.h"

class Tank : public GameObject
{
public:
	Tank(GLuint programID, std::string baseStl, std::string kuppelStl, std::string rohr);
	~Tank();
	void render();
	void update();
private:
	GLuint vaoID;
	GLuint vbo[2];
	GLuint vb_size;
	GLuint vaoIDKuppel;
	GLuint vboKuppel[2];
	GLuint vb_sizeKuppel;
	GLuint vaoIDRohr;
	GLuint vboRohr[2];
	GLuint vb_sizeRohr;
	void initializeBuffers();
	void cleanupBuffers();
	bool onCollissionEnter();
	std::string nickname;
	float speed;
	float reloadTime;
	int health;
	std::string kuppelStl;
	std::string rohrStl;
	glm::vec3 kupelRotation;
	glm::mat4 model;
};

#endif // !TANK_H
