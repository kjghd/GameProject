#pragma once

#include "DataTypes.h"
#include "Camera.h"
#include "Player.h"
#include "Input.h"
#include "Prefabs.h"

#include <vector>
//#include <stack>


class Scene
{
	Input* pInput;
	Camera* pCurrentCamera;
	std::vector<GameObject*> vpSpawnQueue;
	std::vector<size_t> vDestroyQueue;


	void Collision();


public:
	std::vector<GameObject*> vpGameObjects;

	int current_prefab;

	Player* pPlayer;

	PrefabList prefabs;

	Scene(Input* pInput);

	Camera& GetCamera();
	Input& GetInput();

	void Update(float deltaTime);

	void QueueToSpawn(int prefab, game::Float2 location = {0,0});
	void QueueToDestroy(size_t tileIndex);
};