#pragma once

#include "DataTypes.h"
#include "Camera.h"
#include "Player.h"
#include "ScreenObject.h"
#include "Input.h"
#include "Prefabs.h"

#include <vector>
//#include <stack>

enum SceneState
{
	SState_Run,
	SState_Pause
};

class Scene
{
	Input* pInput;
	Camera* pCurrentCamera;
	std::vector<WorldObject*> vpSpawnQueue;
	std::vector<size_t> vDestroyQueue;


	void Collision();


public:
	int state;

	int current_prefab;
	PrefabList prefabs;

	std::vector<WorldObject*> vpGameObjects;
	std::vector<ScreenObject*> vpScreenObjects;
	Player* pPlayer;
	//ScreenObject* ui_background;
	//ScreenObject* button_resume;
	//ScreenObject* button_mainMenu;

	Scene(Input* pInput);
	~Scene();

	Camera& GetCamera();
	Input& GetInput();

	void Update(float deltaTime);

	void QueueToSpawn(int prefab, game::Float2 location = {0,0});
	void QueueToDestroy(size_t tileIndex);

	void SpawnObjects();
	void DestroyObjects();
};
