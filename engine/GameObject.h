#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

// Include OpenGL Stuff
#include <glm/glm.hpp>
#include <GL/glew.h>

// std includes
#include <string>

class GameObject {
	public:
		GameObject(GLuint programID);
		virtual ~GameObject();
		virtual void update() = 0;
		virtual void render() = 0;
		virtual bool onCollisionEnter() = 0;
	protected:
		glm::vec3 position;
		glm::vec3 rotation;
		glm::vec3 scale;
		GLuint programID;
		void parseStl(std::string path);
		virtual void inializeBuffers() = 0;
		virtual void cleanupBuffers() = 0;
		
		
};

#endif // !GAMEOBJECT_H
