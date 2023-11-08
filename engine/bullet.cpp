#include "./engine/bullet.h"
#include "GameObject.h"
#include <iostream>
#include <glm/gtc/matrix_transform.hpp>

Bullet::Bullet(float speed, int damage, glm::vec3 direction, glm::vec3 startPos, GLuint programID, std::string stlPath, std::string name) : GameObject(programID, name)
{
	this->speed = speed;
	this->direction = direction;
	this->position = startPos;
	this->damage = damage;
	this->stlPath = stlPath;
	this->lifeTime = 0;
	this->upwardsForce = 9.81f;
	this->initializeBuffers();
}

Bullet::~Bullet()
{
	
}

void Bullet::update(float deltaTime)
{
	lifeTime += deltaTime;

	if (lifeTime > 5 || position.y <= -5.6f) {
		destroyedFlag = true;
	}
	
	// change position y based on upwardForce, deltaTime and direction.y
	// only if rohr is already exited
	if (lifeTime > 0.35f) {
		float deltaForce = upwardsForce * deltaTime * direction.y * 10.0f;
		position.y += deltaForce;
		// change upwardForce based on gravity
		upwardsForce -= 9.81f * deltaTime;
	}
	
	
	
	glUseProgram(programID);
	
	float forwardMomentum = direction.y;
	if (forwardMomentum < 0)
		forwardMomentum *= -1.0f;
	forwardMomentum = 1.0f -  forwardMomentum;
			

	position.x += direction.x * speed * deltaTime * forwardMomentum;
	position.y += direction.y * speed * deltaTime;
	position.z += direction.z * speed * deltaTime * forwardMomentum;


	glm::mat4 model = glm::mat4(1.0f);

	model = glm::rotate(model, -1.5708f, glm::vec3(1.0f, 0.0f, 0.0f));

	glm::mat4 transformation;//additional transformation for the model
	transformation[0][0] = 10.0; transformation[1][0] = 0.0; transformation[2][0] = 0.0; transformation[3][0] = position.x;
	transformation[0][1] = 0.0; transformation[1][1] = 10.0; transformation[2][1] = 0.0; transformation[3][1] = position.y;
	transformation[0][2] = 0.0; transformation[1][2] = 0.0; transformation[2][2] = 10.0; transformation[3][2] = position.z;
	transformation[0][3] = 0.0; transformation[1][3] = 0.0; transformation[2][3] = 0.0; transformation[3][3] = 1.0;

	model = transformation * model;

	GLuint matrixID = glGetUniformLocation(programID, "model");
	glUniformMatrix4fv(matrixID, 1, GL_FALSE, &model[0][0]);
}

void Bullet::render()
{
	if (lifeTime < 0.35f)
		return;
	
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vb);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glDrawArrays(GL_TRIANGLES, 0, vb_size * 3);
}

bool Bullet::onCollissionEnter(std::shared_ptr<GameObject> collissionObj)
{
	// get name of collission object
	std::string name = collissionObj->getName();
	
	// check if collission object is a obstacle=
	if (name == "obstacle") {
	
		// the bullet shouldnt be destroyed in an instance because maybe a player is inside it
		// so we just increase the lifetime to 4.6f
		if (lifeTime < 4.6f)
			lifeTime = 4.6f;
	}

	return false;
}

void Bullet::initializeBuffers() {

	glUseProgram(programID);

	glGenVertexArrays(1, &vaoID);
	glBindVertexArray(vaoID);
	
	std::vector< glm::vec3 > vertices = std::vector< glm::vec3 >();
	std::vector< glm::vec3 > normals = std::vector< glm::vec3 >();

	parseStl(vertices, normals, stlPath);
	vb_size = vertices.size() * sizeof(glm::vec3);

	std::cout << "Loaded stl file with " << vb_size << " vertices" << std::endl;

	// Create and populate the buffer objects
	glGenBuffers(1, &vb);

	// fill vertex buffer
	glBindBuffer(GL_ARRAY_BUFFER, vb);
	glBufferData(GL_ARRAY_BUFFER, vb_size, &vertices[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	
}

void Bullet::cleanupBuffers() {
	
}