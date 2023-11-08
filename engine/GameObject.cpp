#include "GameObject.h"
#include "engine/parse_stl.h"

GameObject::GameObject(GLuint programID, std::string name)
{
	this->programID = programID;
    this->colliderSphere = std::make_shared<ColliderSphere>(6.0f);
    this->name = name;
}

GameObject::~GameObject()
{
}

std::string GameObject::getName() {
	return name; 
}

void GameObject::setName(std::string newName) {
	name = newName;
}

bool GameObject::isDestroyed() {
	return destroyedFlag;
}

std::shared_ptr<ColliderSphere> GameObject::getColliderSphere() {
    
	colliderSphere->setCenter(position);
	return colliderSphere;
}

void GameObject::parseStl(std::vector< glm::vec3 >& vertices, std::vector< glm::vec3 >& normals, std::string stl_file_name) {
    
    stl::stl_data info = stl::parse_stl(stl_file_name);
    std::vector<stl::triangle> triangles = info.triangles;
    
    for (int i = 0; i < info.triangles.size(); i++) {
        stl::triangle t = info.triangles.at(i);
        glm::vec3 triangleNormal = glm::vec3(t.normal.x,
            t.normal.y,
            t.normal.z);
        
        //add vertex and normal for point 1:
        vertices.push_back(glm::vec3(t.v1.x, t.v1.y, t.v1.z));
        normals.push_back(triangleNormal);
        
        //add vertex and normal for point 2:
        vertices.push_back(glm::vec3(t.v2.x, t.v2.y, t.v2.z));
        normals.push_back(triangleNormal);
        
        //add vertex and normal for point 3:
        vertices.push_back(glm::vec3(t.v3.x, t.v3.y, t.v3.z));
        normals.push_back(triangleNormal);
    }
}

GLuint GameObject::getProgrammId() {
    return programID;
}