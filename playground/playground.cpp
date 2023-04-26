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

std::vector<std::shared_ptr<GameObject>> gameObjects;

int main( void )
{
  //Initialize window
  bool windowInitialized = initializeWindow();
  if (!windowInitialized) return -1;

  initalizeVPTransformation();

  // Add depth buffer
  glEnable(GL_DEPTH_TEST);

  // Create and compile our GLSL program from the shaders
  programID = LoadShaders("SimpleVertexShader.vertexshader", "SimpleFragmentShader.fragmentshader");
  GLuint ground = LoadShaders("../engine/GroundVShader.vertexshader", "../engine/GroundFShader.fragmentshader");

  std::shared_ptr<GameObject> mainTankPtr = std::make_shared<Tank>(programID, "../models/base.stl", "../models/kuppel.stl", "../models/rohr.stl");
  std::shared_ptr<GameObject> grd = std::make_shared<Ground>(ground, "../models/ground.stl");
  gameObjects.push_back(mainTankPtr);
  gameObjects.push_back(grd);
  //start animation loop until escape key is pressed
	do{

    updateAnimationLoop();

	} // Check if the ESC key was pressed or the window was closed
	while( glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS &&
		   glfwWindowShouldClose(window) == 0 );

	
  //Cleanup and close window
  cleanupVertexbuffer();
  glDeleteProgram(programID);
	closeWindow();
  
	return 0;
}

void updateAnimationLoop()
{
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


  for (int i = 0; i < gameObjects.size(); i++) {
      gameObjects.at(i)->update();

      initalizeVPTransformation();
      
	  gameObjects.at(i)->render();
  }
  
  std::vector<std::shared_ptr<GameObject>> bullets = ObjectPool::getGameObjects();

  for (int i = 0; i < bullets.size(); i++) {
      
	  bullets.at(i)->update();

	  initalizeVPTransformation();
      
	  bullets.at(i)->render();
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
  window = glfwCreateWindow(1024, 768, "Tutorial 02 - Red triangle", NULL, NULL);
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
    
    glm::mat4 projectionMatrix = glm::perspective(glm::radians(45.0f), 4.0f / 3.0f, 0.1f, 500.0f);
    
    glm::vec3 cameraPos = glm::vec3(200, 20, 0);
    glm::mat4 viewMatrix = glm::lookAt(
        cameraPos,
        glm::vec3(0, 0, 0),
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

