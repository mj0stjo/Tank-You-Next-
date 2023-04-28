#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

// Include OpenGL Stuff
#include <glm/glm.hpp>
#include <GL/glew.h>

// std includes
#include <string>
#include <vector>

#include "collidersphere.h"

class ColliderSphere;

class GameObject {
	public:
		GameObject(GLuint programID, std::string name);
		virtual ~GameObject();
		virtual void update(float deltaTime) = 0;
		virtual void render() = 0;
		virtual bool onCollissionEnter(std::shared_ptr<GameObject> collissionObj) = 0;
		bool isDestroyed();
		std::shared_ptr<ColliderSphere> getColliderSphere();
		std::string getName();
	protected:
		std::string name;
		std::shared_ptr<ColliderSphere> colliderSphere;
		glm::vec3 position;
		glm::vec3 rotation;
		glm::vec3 scale;
		std::string stlPath;
		GLuint programID;
		void parseStl(std::vector< glm::vec3 >& vertices, std::vector< glm::vec3 >& normals, std::string stl_file_name);
		virtual void initializeBuffers() = 0;
		virtual void cleanupBuffers() = 0;
		bool destroyedFlag = false;
};

#endif // !GAMEOBJECT_H
