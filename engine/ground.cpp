#include "ground.h"
#include "GameObject.h"
#include <iostream>
#include <glm/gtc/matrix_transform.hpp>
#define STB_IMAGE_IMPLEMENTATION
#include <engine/stb_image.h>

Ground::Ground(GLuint programmID, std::string stlPath):GameObject(programmID, "ground") {
	this->stlPath = stlPath;
	rotation.x = -1.5708f;
	rotation.z = -1.5708f;
	
	position.y = -5.6f;

	this->colliderSphere->setCenter(glm::vec3(10000.0f, 10000.0f, 10000.0f));

	initializeBuffers();
}

Ground::~Ground() {
}

void Ground::update(float deltaTime) {

	glUseProgram(programID);
	
	glm::mat4 model = glm::mat4(1.0f);
	
	model = glm::rotate(model, rotation.x, glm::vec3(1.0f,0.0f, 0.0f));
	model = glm::rotate(model, rotation.z, glm::vec3(0.0f,0.0f, 1.0f));
	
	glm::mat4 transformation;//additional transformation for the model
	transformation[0][0] = 20.0; transformation[1][0] = 0.0; transformation[2][0] = 0.0; transformation[3][0] = position.x;
	transformation[0][1] = 0.0; transformation[1][1] = 1.0; transformation[2][1] = 0.0; transformation[3][1] = position.y;
	transformation[0][2] = 0.0; transformation[1][2] = 0.0; transformation[2][2] = 20.0; transformation[3][2] = position.z;
	transformation[0][3] = 0.0; transformation[1][3] = 0.0; transformation[2][3] = 0.0; transformation[3][3] = 1.0;
	
	
	model = transformation * model;
	
	GLuint matrixID = glGetUniformLocation(programID, "model");
	glUniformMatrix4fv(matrixID, 1, GL_FALSE, &model[0][0]);
	
}

void Ground::render() {

	

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);
	glUniform1i(textureSamplerID, 0);
	glEnableVertexAttribArray(2);
	glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
	glVertexAttribPointer(
		2,
		2,
		GL_FLOAT,
		GL_FALSE,
		0,
		(void*)0
	);
	
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vb);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glDrawArrays(GL_TRIANGLES, 0, vb_size * 3);

	

}

void Ground::initializeBuffers() {

	data = stbi_load("../models/groundTexture.png", &width, &height, &nrChannels, 4);
	
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

	textureSamplerID = glGetUniformLocation(programID, "myTextureSampler");

	static const GLfloat g_uv_buffer_data[] = {
		0.0f, 0.0f,
		0.0f, 1.0f,
		1.0f, 1.0f,
		0.0f, 0.0f,
		1.0f, 1.0f,
		1.0f, 0.0f
	};

	glGenBuffers(1, &uvbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_uv_buffer_data), g_uv_buffer_data, GL_STATIC_DRAW);

	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	//glTexSubImage2D(GL_TEXTURE0, 0, 0, 0, width, height, GL_RGB, GL_UNSIGNED_BYTE, data );
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	
}

void Ground::cleanupBuffers() {
	
}

bool Ground::onCollissionEnter(std::shared_ptr<GameObject> collissionObj) {
	return false;
}

