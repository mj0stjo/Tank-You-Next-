#include "tank.h"
#include "GameObject.h"
#include <iostream>
#include <glm/gtc/matrix_transform.hpp>


Tank::Tank(GLuint programmID, std::string stlPath):GameObject(programmID) {
	this->programID = programID;
	this->stlPath = stlPath;
	
	rotation.x = -1.5708f;
	rotation.z = -1.5708f;
	initializeBuffers();
}

Tank::~Tank() {
}

void Tank::update() {

	glUseProgram(programID);
	
	glm::mat4 model = glm::mat4(1.0f);

	rotation.z += 0.001f;
	position.x += 0.011f;
	//default rotation for the model
	model = glm::rotate(model, rotation.x, glm::vec3(1.0f,0.0f, 0.0f));

	glm::mat4 transformation;//additional transformation for the model
	transformation[0][0] = 1.0; transformation[1][0] = 0.0; transformation[2][0] = 0.0; transformation[3][0] = position.x;
	transformation[0][1] = 0.0; transformation[1][1] = 1.0; transformation[2][1] = 0.0; transformation[3][1] = position.y;
	transformation[0][2] = 0.0; transformation[1][2] = 0.0; transformation[2][2] = 1.0; transformation[3][2] = position.z;
	transformation[0][3] = 0.0; transformation[1][3] = 0.0; transformation[2][3] = 0.0; transformation[3][3] = 1.0;
	
	model = glm::rotate(model, rotation.z, glm::vec3(0.0f, 0.0f, 1.0f));
	
	model = transformation * model;
	
	GLuint matrixID = glGetUniformLocation(programID, "model");
	glUniformMatrix4fv(matrixID, 1, GL_FALSE, &model[0][0]);
	
}

void Tank::render() {
	
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glDrawArrays(GL_TRIANGLES, 0, vb_size * 3);

	

}

void Tank::initializeBuffers() {

	glUseProgram(programID);
	
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

}

void Tank::cleanupBuffers() {
	glDeleteBuffers(2, vbo);
	glDeleteVertexArrays(1, &vaoID);
}

bool Tank::onCollissionEnter() {
	return false;
}

