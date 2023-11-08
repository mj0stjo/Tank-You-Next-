#pragma comment(lib, "winmm.lib")
#include "playground.h"


// Include standard headers
#include <stdio.h>
#include <stdlib.h>

// Include GLFW
#include <glfw3.h>
GLFWwindow* window;

// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
using namespace glm;

#include <common/shader.hpp>

#include <vector>
#include <memory>

#include "engine/GameObject.h"
#include "engine/Tank.h"
#include "engine/Ground.h"
#include "engine/keyboardinput.h"
#include "engine/objectpool.h"
#include "network/NetworkManager.h"
#include "engine/obstacle.h"
#include "engine/ui.h"

//parsing config
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <unordered_map>


#include<mmsystem.h>


std::vector<std::shared_ptr<Tank>> networkTanks;
std::vector<std::shared_ptr<GameObject>> obstacles;
std::shared_ptr<Tank>  mainTank;
std::shared_ptr<Ui> ui;
float applicationStartTime;
float lastFrameTime;

std::shared_ptr<NetworkManager> netMngr;

int main(void)
{

	// Play sound
	//PlaySound(TEXT("../sound/pickup.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);

	//mciSendString("open \"../sound/music.mp3\" type mpegvideo alias mp3", NULL, 0, NULL);
	//mciSendString("play mp3 repeat", NULL, 0, NULL);
	
	// play a second sound
	//PlaySound(TEXT("../sound/ambient.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);

	
	// read from config.txt
	std::ifstream configFile("../config.txt");
	std::string line;
	std::unordered_map<std::string, std::string> config;

	if (configFile.is_open()) {
		while (getline(configFile, line)) {
			std::istringstream is_line(line);
			std::string key;
			if (getline(is_line, key, '=')) {
				std::string value;
				if (getline(is_line, value)) {
					config[key] = value;
				}
			}
		}
		configFile.close();
	}
	else {
		std::cerr << "Unable to open config.txt" << std::endl;
		return 1;
	}

	// read color from config file
	glm::vec3 color = glm::vec3(
		std::stof(config["tankColorR"]),
		std::stof(config["tankColorG"]),
		std::stof(config["tankColorB"])
	);
	
	std::string ip = config["ip"];

	
	
	
	//Initialize window
	bool windowInitialized = initializeWindow();
	if (!windowInitialized) return -1;

	applicationStartTime = (float)glfwGetTime();


	// Add depth buffer
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Create and compile our GLSL program from the shaders
	programID = LoadShaders("SimpleVertexShader.vertexshader", "SimpleFragmentShader.fragmentshader");
	GLuint ground = LoadShaders("../engine/GroundVShader.vertexshader", "../engine/GroundFShader.fragmentshader");
	
	GLuint ui_id = LoadShaders("../engine/UiVShader.vertexshader", "../engine/UiFShader.fragmentshader");

	mainTank = std::make_shared<Tank>(programID, "../models/base.stl", "../models/kuppel.stl", "../models/rohr.stl");
	mainTank->setColor(color);
	
	networkTanks.push_back(std::make_shared<Tank>(programID, "../models/base.stl", "../models/kuppel.stl", "../models/rohr.stl"));
	networkTanks.push_back(std::make_shared<Tank>(programID, "../models/base.stl", "../models/kuppel.stl", "../models/rohr.stl"));
	networkTanks.push_back(std::make_shared<Tank>(programID, "../models/base.stl", "../models/kuppel.stl", "../models/rohr.stl"));
	networkTanks.push_back(std::make_shared<Tank>(programID, "../models/base.stl", "../models/kuppel.stl", "../models/rohr.stl"));
	networkTanks.push_back(std::make_shared<Tank>(programID, "../models/base.stl", "../models/kuppel.stl", "../models/rohr.stl"));
	networkTanks.push_back(std::make_shared<Tank>(programID, "../models/base.stl", "../models/kuppel.stl", "../models/rohr.stl"));
	networkTanks.push_back(std::make_shared<Tank>(programID, "../models/base.stl", "../models/kuppel.stl", "../models/rohr.stl"));
	networkTanks.push_back(std::make_shared<Tank>(programID, "../models/base.stl", "../models/kuppel.stl", "../models/rohr.stl"));
	networkTanks.push_back(std::make_shared<Tank>(programID, "../models/base.stl", "../models/kuppel.stl", "../models/rohr.stl"));
	networkTanks.push_back(std::make_shared<Tank>(programID, "../models/base.stl", "../models/kuppel.stl", "../models/rohr.stl"));
	networkTanks.push_back(std::make_shared<Tank>(programID, "../models/base.stl", "../models/kuppel.stl", "../models/rohr.stl"));
	networkTanks.push_back(std::make_shared<Tank>(programID, "../models/base.stl", "../models/kuppel.stl", "../models/rohr.stl"));

	for (int i = 0; i < networkTanks.size(); i++) {
		networkTanks[i]->setPosition(vec3(0, -50, 0));
	}
	
	std::shared_ptr<GameObject> grd = std::make_shared<Ground>(ground, "../models/ground.stl");
	std::shared_ptr<GameObject> o1 = std::make_shared<Obstacle>(ground, "../models/rock.stl", "../models/benis.png", 100.0f, 2.0f, 0.0f, 10.0f, 10.0f, 1.0f);
	std::shared_ptr<GameObject> o2 = std::make_shared<Obstacle>(ground, "../models/rock.stl", "../models/rockTexture.png", 70.0f, 2.0f, 0.0f, 10.0f, 10.0f, 10.0f);
	std::shared_ptr<GameObject> o3 = std::make_shared<Obstacle>(ground, "../models/rock.stl", "../models/benis.png", -20.0f, 2.0f, 50.0f, 10.0f, 10.0f, 10.0f);
	std::shared_ptr<GameObject> o4 = std::make_shared<Obstacle>(ground, "../models/rock.stl", "../models/rockTexture.png", 30.0f, 2.0f, -80.0f, 10.0f, 10.0f, 10.0f);
	obstacles.push_back(grd);
	obstacles.push_back(o1);
	obstacles.push_back(o2);
	obstacles.push_back(o3);
	obstacles.push_back(o4);

	

	// create ui
	ui = std::make_shared<Ui>(ui_id, "../models/ground.stl"); 
	//obstacles.push_back(ui);
	
	//Initialize Network
	netMngr = std::make_shared<NetworkManager>(mainTank, networkTanks);
	if (ip == "server")
		netMngr->startServer();
	else
		netMngr->startClient(ip);

	//start animation loop until escape key is pressed
	do {

		updateAnimationLoop();

	} // Check if the ESC key was pressed or the window was closed
	while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
		glfwWindowShouldClose(window) == 0);


	//Cleanup and close window
	cleanupVertexbuffer();
	glDeleteProgram(programID);
	closeWindow();

	return 0;
}

void updateAnimationLoop()
{
	float deltaTime = (float)glfwGetTime() - lastFrameTime;

	lastFrameTime = (float)glfwGetTime();

	netMngr->synchronize();

	// change window name

	std::string windowName = "Destroyed Count: " + std::to_string(mainTank->getNoobTimes());
	glfwSetWindowTitle(window, windowName.c_str());


	KeyboardInput::setKey('W', glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS);
	KeyboardInput::setKey('A', glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS);
	KeyboardInput::setKey('S', glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS);
	KeyboardInput::setKey('D', glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS);

	// arrow keys
	KeyboardInput::setKey('I', glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS);
	KeyboardInput::setKey('J', glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS);
	KeyboardInput::setKey('K', glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS);
	KeyboardInput::setKey('L', glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS);
	KeyboardInput::setKey('R', glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS);

	// space bar
	KeyboardInput::setKey('_', glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS);

	// Clear the screen
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


	for (int i = 0; i < obstacles.size(); i++) {
		obstacles.at(i)->update(deltaTime);

		initalizeVPTransformation(obstacles.at(i)->getProgrammId());

		obstacles.at(i)->render();

		// check collision with mainTank
		if (mainTank->getColliderSphere()->checkCollision(obstacles.at(i)->getColliderSphere())) {
			mainTank->onCollissionEnter(obstacles.at(i));
		}

	}

	if (!mainTank->getDestroyed()) {
		mainTank->update(deltaTime);
		initalizeVPTransformation(mainTank->getProgrammId());
		mainTank->render();
	}
	else {
		if (KeyboardInput::IsPressed('R')) {
			mainTank->respawn();
		}
	}
	


	////////////////////////////////////////////////////////////////////////
	
	for (int i = 0; i < networkTanks.size(); i++) {
		initalizeVPTransformation(networkTanks[i]->getProgrammId());
		networkTanks[i]->render();
	}
	////////////////////////////////////////////////////////////////////////
	
	std::vector<std::shared_ptr<GameObject>> bullets = ObjectPool::getGameObjects();

	for (int i = 0; i < bullets.size(); i++) {

		bullets.at(i)->update(deltaTime);

		initalizeVPTransformation(bullets.at(i)->getProgrammId());

		bullets.at(i)->render();

		// check collision with mainTank
		if (mainTank->getColliderSphere()->checkCollision(bullets.at(i)->getColliderSphere())) {
			mainTank->onCollissionEnter(bullets.at(i));
		}
		
		// check collision with obstacles
		for (int j = 0; j < obstacles.size(); j++) {
			if (obstacles.at(j)->getColliderSphere()->checkCollision(bullets.at(i)->getColliderSphere())) {
				//obstacles.at(j)->onCollissionEnter(bullets.at(i));
				bullets.at(i)->onCollissionEnter(obstacles.at(j));
			}
		}
	}


	std::shared_ptr<Tank> tankPointer = std::static_pointer_cast<Tank>(mainTank);
	float rot = tankPointer->getKupelRotation().z + tankPointer->getRotation().z;

	initalizeVPTransformationUi(ui->getProgrammId(), mainTank->getDestroyed());
	ui->setRotation(rot);
	ui->setPosition(tankPointer->getPosition().x, tankPointer->getPosition().z);

	ui->render();



	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);

	// Swap buffers
	glfwSwapBuffers(window);
	glfwPollEvents();

}

bool initializeWindow()
{
	// Initialise GLFW
	if (!glfwInit())
	{
		fprintf(stderr, "Failed to initialize GLFW\n");
		getchar();
		return false;
	}

	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Open a window and create its OpenGL context
	window = glfwCreateWindow(1300, 1000, "Tutorial 02 - Red triangle", NULL, NULL);
	if (window == NULL) {
		fprintf(stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n");
		getchar();
		glfwTerminate();
		return false;
	}
	glfwMakeContextCurrent(window);

	// Initialize GLEW
	glewExperimental = true; // Needed for core profile
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
		getchar();
		glfwTerminate();
		return false;
	}

	// Ensure we can capture the escape key being pressed below
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

	// Dark blue background
	glClearColor(0.0f, 0.0f, 0.4f, 0.0f);
	return true;
}

void initalizeVPTransformation(GLuint progID) {

	glUseProgram(progID);


	GLuint viewMatrixID = glGetUniformLocation(progID, "view");
	GLuint projectionMatrixID = glGetUniformLocation(progID, "projection");

	// cast mainTank from GameObject to Tank
	std::shared_ptr<Tank> tankPointer = std::static_pointer_cast<Tank>(mainTank);

	float cameraY = 32.0f;
	glm::vec3 tankPos = tankPointer->getPosition();
	glm::vec3 cameraPos = glm::vec3(tankPos.x, cameraY, tankPos.z);

	float cameraDistance = -tankPointer->getKupelRotation().y *50 + 120.0f;
	// place camera behin tank based on tank rotation
	cameraPos.x += cameraDistance * sin(tankPointer->getRotation().z + tankPointer->getKupelRotation().z - 1.5718f);
	cameraPos.z += cameraDistance * cos(tankPointer->getRotation().z + tankPointer->getKupelRotation().z - 1.5718f);



	glm::mat4 projectionMatrix = glm::perspective(glm::radians(45.0f), 4.0f / 3.0f, 0.1f, 500.0f);

	glm::mat4 viewMatrix = glm::lookAt(
		cameraPos,
		tankPointer->getPosition(),
		glm::vec3(0, 1, 0)
	);

	glUniformMatrix4fv(viewMatrixID, 1, GL_FALSE, &viewMatrix[0][0]);
	glUniformMatrix4fv(projectionMatrixID, 1, GL_FALSE, &projectionMatrix[0][0]);
}
void initalizeVPTransformationUi(GLuint progID, bool dest) {

	glUseProgram(progID);

	GLuint matrixID = glGetUniformLocation(progID, "model");
	GLuint viewMatrixID = glGetUniformLocation(progID, "view");
	GLuint projectionMatrixID = glGetUniformLocation(progID, "projection");

	GLuint isDestroyedFlag = glGetUniformLocation(progID, "isDestroyed");
	glUniform1i(isDestroyedFlag, dest);
	

	std::shared_ptr<Tank> tankPointer = std::static_pointer_cast<Tank>(mainTank);
	float cameraY = 32.0f;
	glm::vec3 tankPos = tankPointer->getPosition();
	glm::vec3 cameraPos = glm::vec3(tankPos.x, cameraY, tankPos.z);

	float cameraDistance = -tankPointer->getKupelRotation().y * 50 + 120.0f;

	cameraPos.x += cameraDistance * sin(tankPointer->getRotation().z + tankPointer->getKupelRotation().z - 1.5718f);
	cameraPos.z += cameraDistance * cos(tankPointer->getRotation().z + tankPointer->getKupelRotation().z - 1.5718f);

	glm::mat4 projectionMatrix = glm::perspective(glm::radians(45.0f), 4.0f / 3.0f, 0.1f, 500.0f);

	glm::mat4 viewMatrix = glm::lookAt(
		cameraPos,
		tankPos, // Assuming you want the camera to look at the tank
		glm::vec3(0, 1, 0)
	);

	

	// The UI should be in front of the camera.
	// Calculate the forward vector of the camera from the view matrix.
	glm::vec3 forwardVec = -glm::vec3(viewMatrix[0][2], viewMatrix[1][2], viewMatrix[2][2]);
	glm::vec3 upVec = glm::vec3(viewMatrix[0][1], viewMatrix[1][1], viewMatrix[2][1]);

	// Calculate the position for the UI plane.
	glm::vec3 uiPosition = cameraPos + forwardVec * 10.0f; // 10 units in front of the camera.

	glm::mat4 modelMatrix = glm::mat4(1.0f);
	
	// calculate rotation
	glm::vec3 rotation = glm::vec3(0.0f, 0.0f, 0.0f);
	rotation.x = atan2(forwardVec.y, forwardVec.z);
	rotation.y = atan2(forwardVec.x, forwardVec.z);
	rotation.z = atan2(forwardVec.x, forwardVec.y);

	// Rotate the UI plane to face the camera.
		
	modelMatrix = glm::translate(modelMatrix, uiPosition);
	modelMatrix = glm::rotate(modelMatrix, rotation.y, glm::vec3(0.0f, 1.0f, 0.0f));

	

	// Optionally scale the UI if needed.
	//modelMatrix = glm::scale(modelMatrix, glm::vec3(0.9f, 0.9f, 0.9f));


	glUniformMatrix4fv(matrixID, 1, GL_FALSE, &modelMatrix[0][0]);
	glUniformMatrix4fv(viewMatrixID, 1, GL_FALSE, &viewMatrix[0][0]);
	glUniformMatrix4fv(projectionMatrixID, 1, GL_FALSE, &projectionMatrix[0][0]);
}



bool cleanupVertexbuffer()
{
	// Cleanup VBO
	glDeleteBuffers(1, &vertexbuffer);
	glDeleteVertexArrays(1, &VertexArrayID);
	return true;
}

bool closeWindow()
{
	glfwTerminate();
	return true;
}