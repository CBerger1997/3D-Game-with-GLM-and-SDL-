// \Class used to create an object
// \author Callum Berger

#ifndef _GameObject_
#define _GameObject_

#include "Material.h"
#include "Mesh.h"

#include <string>
#include <gtc/matrix_transform.hpp>

//GameObject Class decleration
class GameObject
{
public:
	GameObject();
	~GameObject();

	void SetMesh(Mesh*);
	void SetMaterial(Material*);

	void SetPosition(float, float, float);

	void SetRotation(float, float, float);

	void Update(float, bool*, float);

	void Draw(glm::mat4, glm::mat4, glm::vec4);

	glm::vec3 GetPosition();

	glm::vec3 GetCollision();

private:

	Mesh *m_mesh;

	Material *m_material;

	glm::mat4 m_modelMatrix;

	glm::vec3 m_position;

	glm::vec3 m_rotation;

	glm::vec3 _rotation;

	glm::vec3 m_collisionBox;

};

#endif // defines _GameObject_