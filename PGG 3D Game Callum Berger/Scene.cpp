//  Scene.cpp
//  Holds all function code for the Scene Class

#include "Scene.h"

#include <iostream>
#include <SDL.h>

#include <cstdlib>
#include <ctime>

//initialisation upon creating a scene object
Scene::Scene()
{
	//used for randomisation
	srand(time(0));

	//initialises variables
	m_playerHealth = 100.0f;

	m_collisionFound = false;
	m_cameraAngleY = 3.141592653589793238462f;

	m_viewMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, -3.5f));

	m_projMatrix = glm::perspective(45.0f, 1.0f, 0.1f, 100.0f);

	m_lightPosition = glm::vec4( 0.0f, 0.0f, 0.0f, 1.0f );


	//Player Object initialisation

	m_player = InitialiseObject(m_player, "ShipTexture.bmp", "Ship.obj");

	//meteorite Object initialisation

	//creates the material and mesh for the object and sets the to the object
	Material* model2Material = new Material();

	model2Material->ShaderCreation("VertexShader.txt", "FragmentShader.txt");

	model2Material->SetTexture("MeteoriteTexture.bmp");

	Mesh* model2Mesh = new Mesh();

	model2Mesh->LoadOBJ("Meteorite.obj");

	//loop used to set the same material and mesh to all meteorite objects with random positions
	for (int i = 0; i < 40; i++)
	{
		m_meteorite[i] = new GameObject();
		m_meteorite[i]->SetMaterial(model2Material);
		m_meteorite[i]->SetMesh(model2Mesh);

		float x1, y1, z1;

		x1 = 3.5f + (rand() / (RAND_MAX / (-3.5f - 3.5f) + 1));

		y1 = 6.0f + (rand() / (RAND_MAX / (-1.0f - 6.0f) + 1));

		z1 = 150.0f + (rand() / (RAND_MAX / (150.0f - 250.0f) + 1));

		m_meteorite[i]->SetPosition(1.0f, 1.0f, 0.0f);
	}

}

Scene::~Scene()
{

}

//Function used to initialise an object and return it
GameObject* Scene::InitialiseObject(GameObject* model, std::string textureLocation, std::string objectLocation)
{
	//creates the gameobject
	model = new GameObject();

	//creates the material for the gameobject
	Material *modelMaterial = new Material();

	//sets the shaders for the material
	modelMaterial->ShaderCreation("VertexShader.txt", "FragmentShader.txt");

	//sets the texture for the material
	modelMaterial->SetTexture(textureLocation);

	//sets the material to the gameobject
	model->SetMaterial(modelMaterial);

	//creates the mesh for the gameobject
	Mesh *modelMesh = new Mesh();

	//loads the mesh from an object file
	modelMesh->LoadOBJ(objectLocation);

	//sets the mesh to the gameobject
	model->SetMesh(modelMesh);

	//returns the gameobject
	return model;
}

//Function that updates the scene
void Scene::Update(float deltaTime, bool* movement)
{
	//function used to update the movement and rotation of the player
	m_player->Update(deltaTime, movement, m_sceneTimeCounter);

	//variable used to store the position of the camera relative to the player's position
	glm::vec3 cameraPosition;

	//sets the cameraPosition variable equal to the player's position
	cameraPosition = m_player->GetPosition();

	//Updates the view matrix to the newly updated camera position and rotation
	m_viewMatrix = glm::rotate(glm::translate(glm::mat4(1.0f), glm::vec3(0, -3.5f, cameraPosition.z - 10.0f)), m_cameraAngleY, glm::vec3(0, 1, 0));

	m_lightPosition = glm::vec4(m_player->GetPosition(), 1.0f);

	//for loop used to check whether an object is outside the view of the camera, resetting the objects location
	for (int i = 0; i < 40; i++)
	{
		if (m_player->GetPosition().z > m_meteorite[i]->GetPosition().z + 10.0f)
		{
			//randomisation used to set the location of the object currently outside the view of the camera
			float x1, y1, z1;

			x1 = 3.5f + (rand() / (RAND_MAX / (-3.5f - 3.5f) + 1));

			y1 = 6.0f + (rand() / (RAND_MAX / (-1.0f - 6.0f) + 1));

			z1 = 150.0f + (rand() / (RAND_MAX / (150.0f - 250.0f) + 1));

			m_meteorite[i]->SetPosition(x1, y1, z1 + m_player->GetPosition().z);
		}
	}

	//for loop used to check for collision between the player object and the meteorite objects using box collision
	for (int i = 0; i < 40; i++)
	{

		//checks for collision using the 2 objects collision boxes
		if (m_player->GetPosition().x - (m_player->GetCollision().x / 4) < m_meteorite[i]->GetPosition().x + (m_meteorite[i]->GetCollision().x / 2)
			&& m_player->GetPosition().x + (m_player->GetCollision().x / 4) > m_meteorite[i]->GetPosition().x - (m_meteorite[i]->GetCollision().x / 2)
			&& m_player->GetPosition().y - (m_player->GetCollision().y / 6) < m_meteorite[i]->GetPosition().y + (m_meteorite[i]->GetCollision().y / 6)
			&& m_player->GetPosition().y + (m_player->GetCollision().y / 2) > m_meteorite[i]->GetPosition().y - (m_meteorite[i]->GetCollision().y / 2)
			&& m_player->GetPosition().z - (m_player->GetCollision().z / 2) < m_meteorite[i]->GetPosition().z + (m_meteorite[i]->GetCollision().z / 2)
			&& m_player->GetPosition().z + (m_player->GetCollision().z / 2) > m_meteorite[i]->GetPosition().z - (m_meteorite[i]->GetCollision().z / 2))
		{
			//reduces the player's health when collision is detected
			std::cout << "Player Health: " << m_playerHealth << std::endl;
			m_playerHealth -= 1.0f;
			break;
		}

	}

}

//Model drawing function
GameObject* Scene::Draw(GameObject* model)
{
	//draws the model passed through as a parameter
	model->Draw(m_viewMatrix, m_projMatrix, m_lightPosition);

	//returns the updated model
	return model;
}

//Funciton used to update the camera angle on the y axis
void Scene::ChangeCameraAngleY(float y)
{
	m_cameraAngleY += y;
}

//Get and Set Functions

#pragma region Get and Set Functions

GameObject* Scene::GetModel(int modelValue, bool player)
{
	if (player == true)
	{
		return m_player;
	}
	else
	{
		return m_meteorite[modelValue];
	}

}

void Scene::IncrementSceneTimeCounter()
{
	m_sceneTimeCounter += 0.0001f;
}

float Scene::GetSceneTimeCounter()
{
	return m_sceneTimeCounter;
}

float Scene::GetPlayerHealth()
{
	return m_playerHealth;
}

void Scene::SetGameScore(float scoreVal)
{
	m_score = scoreVal;
}

float Scene::GetGameScore()
{
	return m_score;
}

#pragma endregion

