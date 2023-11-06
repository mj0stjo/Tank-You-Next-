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

std::vector<std::shared_ptr<Tank>> networkTanks;
std::vector<std::shared_ptr<GameObject>> obstacles;
std::shared_ptr<Tank>  mainTank;
float applicationStartTime;
float lastFrameTime;

std::shared_ptr<NetworkManager> netMngr;

int main(void)
{
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

	mainTank = std::make_shared<Tank>(programID, "../models/base.stl", "../models/kuppel.stl", "../models/rohr.stl");
	networkTanks.push_back(std::make_shared<Tank>(programID, "../models/base.stl", "../models/kuppel.stl", "../models/rohr.stl"));
	
	std::shared_ptr<GameObject> grd = std::make_shared<Ground>(ground, "../models/ground.stl");
	std::shared_ptr<GameObject> o1 = std::make_shared<Obstacle>(ground, "../models/rock.stl", "../models/rockTexture.png", 0.0f, 2.0f, 0.0f, 10.0f, 10.0f, 1.0f);
	std::shared_ptr<GameObject> o2 = std::make_shared<Obstacle>(ground, "../models/rock.stl", "../models/rockTexture.png", 70.0f, 2.0f, 0.0f, 10.0f, 10.0f, 10.0f);
	std::shared_ptr<GameObject> o3 = std::make_shared<Obstacle>(ground, "../models/rock.stl", "../models/rockTexture.png", -20.0f, 2.0f, 50.0f, 10.0f, 10.0f, 10.0f);
	std::shared_ptr<GameObject> o4 = std::make_shared<Obstacle>(ground, "../models/rock.stl", "../models/rockTexture.png", 30.0f, 2.0f, -80.0f, 10.0f, 10.0f, 10.0f);
	obstacles.push_back(grd);
	obstacles.push_back(o1);
	obstacles.push_back(o2);
	obstacles.push_back(o3);
	obstacles.push_back(o4);

	//Initialize Network
	netMngr = std::make_shared<NetworkManager>(mainTank, networkTanks);
	netMngr->startServer();
	//netMngr->startClient("25.68.196.228");

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


	KeyboardInput::setKey('W', glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS);
	KeyboardInput::setKey('A', glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS);
	KeyboardInput::setKey('S', glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS);
	KeyboardInput::setKey('D', glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS);

	// arrow keys
	KeyboardInput::setKey('I', glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS);
	KeyboardInput::setKey('J', glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS);
	KeyboardInput::setKey('K', glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS);
	KeyboardInput::setKey('L', glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS);

	// space bar
	KeyboardInput::setKey('_', glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS);

	// Clear the screen
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


	for (int i = 0; i < obstacles.size(); i++) {
		obstacles.at(i)->update(deltaTime);

		initalizeVPTransformation();

		obstacles.at(i)->render();
	}

	mainTank->update(deltaTime);
	initalizeVPTransformation();
	mainTank->render();

	////////////////////////////////////////////////////////////////////////
	//networkTanks[0]->update(deltaTime);
	initalizeVPTransformation();
	networkTanks[0]->render();
	////////////////////////////////////////////////////////////////////////

	std::vector<std::shared_ptr<GameObject>> bullets = ObjectPool::getGameObjects();

	for (int i = 0; i < bullets.size(); i++) {

		bullets.at(i)->update(deltaTime);

		initalizeVPTransformation();

		bullets.at(i)->render();

		// check collision with mainTank
		if (mainTank->getColliderSphere()->checkCollision(bullets.at(i)->getColliderSphere())) {
			mainTank->onCollissionEnter(bullets.at(i));
		}
		
		// check collision with obstacles
		for (int j = 0; j < obstacles.size(); j++) {
			if (obstacles.at(j)->getColliderSphere()->checkCollision(bullets.at(i)->getColliderSphere())) {
				obstacles.at(j)->onCollissionEnter(bullets.at(i));
			}
		}
	}



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

void initalizeVPTransformation() {


	GLuint viewMatrixID = glGetUniformLocation(programID, "view");
	GLuint projectionMatrixID = glGetUniformLocation(programID, "projection");

	// cast mainTank from GameObject to Tank
	std::shared_ptr<Tank> tankPointer = std::static_pointer_cast<Tank>(mainTank);

	float cameraY = 32.0f;
	glm::vec3 tankPos = tankPointer->getPosition();
	glm::vec3 cameraPos = glm::vec3(tankPos.x, cameraY, tankPos.z);

	float cameraDistance = -tankPointer->getKupelRotation().y * 50 + 120.0f;
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