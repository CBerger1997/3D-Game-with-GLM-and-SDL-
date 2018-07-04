// \Class used to create the game scene
// \author Callum Berger

#ifndef _Scene_
#define _Scene_

#include "GameObject.h"

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>

#include <string>

//Scene Class Decleration
class Scene
{
public:
	Scene();
	~Scene();

	void ChangeCameraAngleY(float);

	void Update(float, bool*);

	GameObject* Draw(GameObject*);

	GameObject* GetModel(int, bool);

	GameObject* InitialiseObject(GameObject*, std::string, std::string);

	void IncrementSceneTimeCounter();
	float GetSceneTimeCounter();

	float GetPlayerHealth();

	void SetGameScore(float);
	float GetGameScore();

private:

	float m_playerHealth;

	GameObject* m_player;

	GameObject* m_meteorite[40];

	float m_sceneTimeCounter;

	float m_score;

	bool m_collisionFound;

	glm::mat4 m_viewMatrix;
	glm::mat4 m_projMatrix;

	glm::vec4 m_lightPosition;

	float m_cameraAngleY;

};

#endif // Defines _Scene_