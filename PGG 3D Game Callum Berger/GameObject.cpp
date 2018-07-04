//  GameObject.cpp
//  Holds all function code for the GameObject Class

#include "GameObject.h"

//Initialisation function used upon creation of a gameobject class object
GameObject::GameObject()
{
	//Initialises all variables to be empty
	m_mesh = NULL;

	m_material = NULL;

}

GameObject::~GameObject()
{

}

//Function used to update gameobjects
void GameObject::Update(float deltaTime, bool* movement, float sceneTimeCounter)
{

	//if statements used to check for user input and perform position updates

	//Up
	if (movement[0] & !movement[1])
	{

		//moves the object up
		m_position.y += deltaTime * 3.0f;

		if (m_position.y > 6.0f)
		{
			m_position.y = 6.0f;

		}

		//rotates the object to look up
		m_rotation.x -= deltaTime * 0.75;

		while (m_rotation.x > (3.14159265358979323846 * 2.0))
		{
			m_rotation.x += (float)(3.14159265358979323846 * 2.0);
		}

		if (m_rotation.x < -0.5f)
		{
			m_rotation.x = -0.5f;
		}

	}

	//Down
	else if (movement[1] & !movement[0])
	{

		//moves the object down
		m_position.y -= deltaTime * 3.0f;

		if (m_position.y < -1.0f)
		{
			m_position.y = -1.0f;
		}

		//rotates the object to look down
		m_rotation.x += deltaTime * 0.75;

		while (m_rotation.x >(3.14159265358979323846 * 2.0))
		{
			m_rotation.x -= (float)(3.14159265358979323846 * 2.0);
		}

		if (m_rotation.x > 0.25f)
		{
			m_rotation.x = 0.25f;
		}

	}
	//makes the user rotate back to neutral if no movement detected
	else if (m_rotation.x != 0)
	{
		if (m_rotation.x > 0.01)
		{
			m_rotation.x -= deltaTime * 0.75f;

			while (m_rotation.x > (3.14159265358979323846 * 2.0))
			{
				m_rotation.x += (float)(3.14159265358979323846 * 2.0);
			}
		}
		else if (m_rotation.x < -0.01)
		{
			m_rotation.x += deltaTime * 0.75f;

			while (m_rotation.x >(3.14159265358979323846 * 2.0))
			{
				m_rotation.x -= (float)(3.14159265358979323846 * 2.0);
			}
		}
		else
		{
			m_rotation.x = 0;
		}
	}

	//Left
	if (movement[2] & !movement[3])
	{

		//moves the object left
		m_position.x += deltaTime * 3.0f;

		if (m_position.x > 3.5f)
		{
			m_position.x = 3.5f;
		}

		//rotates the object to turn with the left movement
		m_rotation.z += deltaTime * 0.75f;

		while (m_rotation.z > (3.14159265358979323846 * 2.0))
		{
			m_rotation.z -= (float)(3.14159265358979323846 * 2.0);
		}

		if (m_rotation.z > 0.75f)
		{
			m_rotation.z = 0.75f;
		}

	}

	//Right
	else if (movement[3] & !movement[2])
	{

		//moves the object right
		m_position.x -= deltaTime * 3.0f;

		if (m_position.x < -3.5f)
		{
			m_position.x = -3.5f;
		}

		//rotates the object to turn with the right movement
		m_rotation.z -= deltaTime * 0.75f;

		while (m_rotation.z >(3.14159265358979323846 * 2.0))
		{
			m_rotation.z += (float)(3.14159265358979323846 * 2.0);
		}

		if (m_rotation.z < -0.75f)
		{
			m_rotation.z = -0.75f;
		}

	}
	//makes the user rotate back to neutral if no movement detected
	else if (m_rotation.z != 0)
	{
		if (m_rotation.z > 0.01)
		{
			m_rotation.z -= deltaTime * 0.75f;

			while (m_rotation.z > (3.14159265358979323846 * 2.0))
			{
				m_rotation.z += (float)(3.14159265358979323846 * 2.0);
			}
		}
		else if (m_rotation.z < -0.01)
		{
			m_rotation.z += deltaTime * 0.75f;

			while (m_rotation.z >(3.14159265358979323846 * 2.0))
			{
				m_rotation.z -= (float)(3.14159265358979323846 * 2.0);
			}
		}
		else
		{
			m_rotation.z = 0;
		}
	}

	//moves the player forward relative to the amount of time the program has been played
	m_position.z += (deltaTime * 10.0f + sceneTimeCounter);

}

//Function used to draw the game object
void GameObject::Draw(glm::mat4 viewMatrix, glm::mat4 projMatrix, glm::vec4 lightPosition)
{
	//checks whether the object has a mesh and material
	if (m_mesh != NULL)
	{
		if (m_material != NULL)
		{
			//updates the model matrix relative to the objects position and rotation
			m_modelMatrix = glm::translate(glm::mat4(1.0f), m_position);
			m_modelMatrix = glm::rotate(glm::rotate(m_modelMatrix, m_rotation.z, glm::vec3(0, 0, 1)), m_rotation.x, glm::vec3(1, 0, 0));

			m_material->SetUniforms(m_modelMatrix, viewMatrix, projMatrix, lightPosition);

			//applues the texture
			m_material->ApplyTexture();
		}
		//draws the mesh
		m_mesh->Draw();
	}

}

//Get and Set Functions

#pragma region Get and Set Functions

void GameObject::SetMesh(Mesh *mesh)
{
	float x;
	float y;
	float z;

	x = mesh->GetXWidth();
	y = mesh->GetHeight();
	z = mesh->GetZWidth();

	m_collisionBox = glm::vec3(x, y, z);

	m_mesh = mesh;
}

void GameObject::SetMaterial(Material *material)
{
	m_material = material;
}

void GameObject::SetPosition(float x, float y, float z)
{
	m_position.x = x;
	m_position.y = y;
	m_position.z = z;
}

void GameObject::SetRotation(float x, float y, float z)
{
	m_rotation.x = x;
	m_rotation.y = y;
	m_rotation.z = z;
}

glm::vec3 GameObject::GetPosition()
{
	return m_position;
}

glm::vec3 GameObject::GetCollision()
{
	return m_collisionBox;
}

#pragma endregion