#ifndef TANK_H
#define	TANK_H
#include "./engine/GameObject.h"

class Tank : public GameObject
{
public:
	Tank(GLuint programID, std::string baseStl, std::string kuppelStl, std::string rohr);
	~Tank();
	void render();
	void update(float deltaTime);
	bool onCollissionEnter(std::shared_ptr<GameObject> collissionObj);
	glm::vec3 getPosition();
	void setPosition(glm::vec3 pos);
	glm::vec3 getRotation();
	glm::vec3 getKupelRotation();
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
