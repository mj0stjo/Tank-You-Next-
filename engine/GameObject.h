#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

// Include OpenGL Stuff
#include <glm/glm.hpp>
#include <GL/glew.h>

// std includes
#include <string>
#include <vector>

class GameObject {
	public:
		GameObject(GLuint programID);
		virtual ~GameObject();
		virtual void update() = 0;
		virtual void render() = 0;
		virtual bool onCollissionEnter() = 0;
	protected:
		glm::vec3 position;
		glm::vec3 rotation;
		glm::vec3 scale;
		std::string stlPath;
		GLuint programID;
		void parseStl(std::vector< glm::vec3 >& vertices, std::vector< glm::vec3 >& normals, std::string stl_file_name);
		virtual void initializeBuffers() = 0;
		virtual void cleanupBuffers() = 0;
		
		
};

#endif // !GAMEOBJECT_H
