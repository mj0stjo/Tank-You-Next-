#ifndef TANK_H
#define	TANK_H
#include "./engine/GameObject.h"
#include "./engine/bullet.h"

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
	void setRotation(glm::vec3 rot);
	glm::vec3 getKupelRotation();
	void setKupelRotation(glm::vec3 kRot);
	bool getDestroyed();
	void setDestroyed(bool b);
	void respawn();
	void setColor(glm::vec3 col);
	glm::vec3 getColor();
	int getNoobTimes();

	std::shared_ptr<Bullet> lastBullet;
private:
	int noobTimes;
	glm::vec3 color;
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
	bool destroyed;
};

#endif // !TANK_H
