#include "tank.h"
#include "GameObject.h"
#include <iostream>
#include <glm/gtc/matrix_transform.hpp>
#include "./engine/keyboardinput.h"
#include "bullet.h"
#include <memory>
#include "objectpool.h"
#include <common/shader.hpp>


Tank::Tank(GLuint programmID, std::string baseStl, std::string kuppelStl, std::string rohr):GameObject(programmID, "tank") {
	this->programID = programID;
	this->stlPath = baseStl;
	this->kuppelStl = kuppelStl;
	this->rohrStl = rohr;
	this->speed = 20.0f;
	rotation.x = -1.5708f;
	rotation.z = -1.5708f;
	initializeBuffers();
}

Tank::~Tank() {
}

void Tank::update(float deltaTime) {
	
	reloadTime -= deltaTime;

	

	if (KeyboardInput::IsPressed('_') && reloadTime <= 0) {

		float bulletSpeed = 60.0f;
		int bulletDamage = 10;
		glm::vec2 direction = glm::vec2(cos(rotation.z + kupelRotation.z), -sin(rotation.z + kupelRotation.z));
		glm::vec3 direction3 = glm::vec3(direction.x, -kupelRotation.y, direction.y);
		GLuint bulletShaderID = LoadShaders("../engine/BulletVShader.vertexshader", "../engine/BulletFShader.fragmentshader");
		glm::vec3 bulletPos = glm::vec3(position.x, position.y + 3.75f, position.z);

		std::shared_ptr<GameObject> bullet = std::make_shared<Bullet>(bulletSpeed, bulletDamage, direction3, bulletPos, bulletShaderID, "../models/monke.stl", "localBullet");
		ObjectPool::addGameObject(bullet);
		reloadTime = 1.1f;
		std::cout << "Spawned a bullet" << std::endl;
	}

	if (KeyboardInput::IsPressed('W')) {
		
		glm::vec2 direction = glm::vec2(cos(rotation.z), sin(rotation.z));
		position.x += direction.x * speed * deltaTime;
		position.z -= direction.y * speed * deltaTime;
	}
	
	if (KeyboardInput::IsPressed('S')) {
		glm::vec2 direction = glm::vec2(cos(rotation.z), sin(rotation.z));
		position.x -= direction.x * speed * deltaTime;
		position.z += direction.y * speed * deltaTime;
	}

	if (KeyboardInput::IsPressed('A')) {
		rotation.z += speed/20 * deltaTime;
	}
	
	if (KeyboardInput::IsPressed('D')) {
		rotation.z -= speed / 20 * deltaTime;
	}
	
	if (KeyboardInput::IsPressed('J')) {
		kupelRotation.z += speed / 15 * deltaTime;
	}

	if (KeyboardInput::IsPressed('L')) {
		kupelRotation.z -= speed / 15 * deltaTime;
	}

	if (KeyboardInput::IsPressed('K')) {
		if (kupelRotation.y < 0.165f) 
			kupelRotation.y += speed / 20 * deltaTime;
	}
	
	if (KeyboardInput::IsPressed('I')) {
		if (kupelRotation.y > -0.55f)
			kupelRotation.y -= speed / 20 * deltaTime;
	}

	
	
	
}

void Tank::render() {

	model = glm::mat4(1.0f);


	//default rotation for the model
	model = glm::rotate(model, rotation.x, glm::vec3(1.0f,0.0f, 0.0f));

	glm::mat4 transformation;//additional transformation for the model
	transformation[0][0] = 1.0; transformation[1][0] = 0.0; transformation[2][0] = 0.0; transformation[3][0] = position.x;
	transformation[0][1] = 0.0; transformation[1][1] = 1.0; transformation[2][1] = 0.0; transformation[3][1] = position.y;
	transformation[0][2] = 0.0; transformation[1][2] = 0.0; transformation[2][2] = 1.0; transformation[3][2] = position.z;
	transformation[0][3] = 0.0; transformation[1][3] = 0.0; transformation[2][3] = 0.0; transformation[3][3] = 1.0;
	
	model = glm::rotate(model, rotation.z, glm::vec3(0.0f, 0.0f, 1.0f));
	
	model = transformation * model;

	glUseProgram(programID);
	
	GLuint matrixID = glGetUniformLocation(programID, "model");
	glUniformMatrix4fv(matrixID, 1, GL_FALSE, &model[0][0]);
	
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glDrawArrays(GL_TRIANGLES, 0, vb_size * 3);
	
	// set translation to 0 again
	
	model[3][0] = 0.0f;
	model[3][1] = 0.0f;
	model[3][2] = 0.0f;
	
	// add kuppel rotation

	model = glm::rotate(model, kupelRotation.z, glm::vec3(0.0f, 0.0f, 1.0f));
	
	// add translation for kuppel
	
	model[3][0] = position.x;
	model[3][1] = position.y;
	model[3][2] = position.z;

	// send new matrix to shader

	matrixID = glGetUniformLocation(programID, "model");
	glUniformMatrix4fv(matrixID, 1, GL_FALSE, &model[0][0]);

	// render kuppel
	
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vboKuppel[0]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, vboKuppel[1]);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glDrawArrays(GL_TRIANGLES, 0, vb_sizeKuppel * 3);
	

	// set translation to 0 again
	
	model[3][0] = 0.0f;
	model[3][1] = 0.0f;
	model[3][2] = 0.0f;

	// add rotation of rohr
	
	model = glm::rotate(model, kupelRotation.y, glm::vec3(0.0f, 1.0f, 0.0f));

	// add translation for rohr
	
	model[3][0] = position.x;
	model[3][1] = position.y;
	model[3][2] = position.z;

	// send new matrix to shader

	matrixID = glGetUniformLocation(programID, "model");
	glUniformMatrix4fv(matrixID, 1, GL_FALSE, &model[0][0]);

	// render rohr
	
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vboRohr[0]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, vboRohr[1]);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glDrawArrays(GL_TRIANGLES, 0, vb_sizeRohr * 3);
	

	

}

void Tank::initializeBuffers() {

	glUseProgram(programID);

	// Create VAO for the base
	
	glGenVertexArrays(1, &vaoID);
	glBindVertexArray(vaoID);

	std::vector< glm::vec3 > vertices = std::vector< glm::vec3 >();
	std::vector< glm::vec3 > normals = std::vector< glm::vec3 >();
	
	parseStl(vertices, normals, stlPath);
	vb_size = vertices.size() * sizeof(glm::vec3);

	std::cout << "Loaded stl file with " << vb_size << " vertices" << std::endl;
	
	// Create and populate the buffer objects
	glGenBuffers(2, vbo);

	// fill vertex buffer
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, vb_size, &vertices[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	glBufferData(GL_ARRAY_BUFFER, vb_size, &normals[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);

	// Create VAO for the kuppel
	
	glGenVertexArrays(1, &vaoIDKuppel);
	glBindVertexArray(vaoIDKuppel);

	vertices = std::vector< glm::vec3 >();
	normals = std::vector< glm::vec3 >();

	parseStl(vertices, normals, kuppelStl);
	vb_sizeKuppel = vertices.size() * sizeof(glm::vec3);

	std::cout << "Loaded stl file (Kuppel) with " << vb_sizeKuppel << " vertices" << std::endl;

	// Create and populate the buffer objects
	glGenBuffers(2, vboKuppel);

	// fill vertex buffer
	glBindBuffer(GL_ARRAY_BUFFER, vboKuppel[0]);
	glBufferData(GL_ARRAY_BUFFER, vb_sizeKuppel, &vertices[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, vboKuppel[1]);
	glBufferData(GL_ARRAY_BUFFER, vb_sizeKuppel, &normals[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);

	// Create VAO for Rohr

	glGenVertexArrays(1, &vaoIDRohr);
	glBindVertexArray(vaoIDRohr);

	vertices = std::vector< glm::vec3 >();
	normals = std::vector< glm::vec3 >();

	parseStl(vertices, normals, rohrStl);
	vb_sizeRohr = vertices.size() * sizeof(glm::vec3);

	std::cout << "Loaded stl file (Rohr) with " << vb_sizeRohr << " vertices" << std::endl;

	// Create and populate the buffer objects
	glGenBuffers(2, vboRohr);

	// fill vertex buffer
	glBindBuffer(GL_ARRAY_BUFFER, vboRohr[0]);
	glBufferData(GL_ARRAY_BUFFER, vb_sizeRohr, &vertices[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, vboRohr[1]);
	glBufferData(GL_ARRAY_BUFFER, vb_sizeRohr, &normals[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	

}

void Tank::cleanupBuffers() {
	glDeleteBuffers(2, vbo);
	glDeleteVertexArrays(1, &vaoID);
}

bool Tank::onCollissionEnter(std::shared_ptr<GameObject> collissionObj) {
	std::string name = collissionObj->getName();
	
	if (name == "localBullet") {
		std::cout << "self hit";
	}
	
	
	return false;
}

glm::vec3 Tank::getPosition() {
	return position;
}

void Tank::setPosition(glm::vec3 pos) {
	position = pos;
}

glm::vec3 Tank::getRotation() {
	return rotation;
}

void Tank::setRotation(glm::vec3 rot) {
	rotation = rot;
}

glm::vec3 Tank::getKupelRotation() {
	return kupelRotation;
}

void Tank::setKupelRotation(glm::vec3 kRot) {
	kupelRotation = kRot;
}

