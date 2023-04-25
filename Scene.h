#pragma once

#include "DataTypes.h"
#include "Camera.h"
#include "Player.h"
#include "ScreenObject.h"
#include "SO_Button.h"
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
	std::vector<GameObject*> vpSpawnQueue;
	std::vector<size_t> vDestroyQueue;
	std::vector<GameObject*> vpGameObjects;

	int state;

	int current_prefab;
	PrefabList prefabs;

	Player* pPlayer;
	ScreenObject* pCursor;
	GameObject* pCursorBox;
	SO_Button* pResume;
	SO_Button* pMainMenu;
	ScreenObject* pCurrentPrefab;


	void Collision();

	void QueueToSpawn(int prefab, game::Float2 location = { 0,0 });
	void QueueToDestroy(size_t tileIndex);

	void SpawnObjects();
	void DestroyObjects();

public:
	Scene(Input* pInput);
	~Scene();

	Camera& GetCamera();
	Input& GetInput();
	Player& GetPlayer();

	void Update(float deltaTime);
};
