#pragma once

#include "DataTypes.h"
#include "Camera.h"
#include "Player.h"
#include "Tile.h"
#include "Ball.h"
#include "Input.h"

#include <vector>
#include <stack>


class Scene
{
	Input* pInput;
	const char* name;
	Camera camera;
	std::stack<GameObject*> vpSpawnQueue;
	std::vector<size_t> vDestroyQueue;

	void Collision();


public:
	std::vector<GameObject*> vpGameObjects;

	int current_prefab;

	Player* pPlayer;

	Scene(Input* pInput);

	Camera& GetCamera();
	Input& GetInput();

	void Update(float deltaTime);

	void QueueToSpawn(int prefab, game::Float2 location = {0,0});
	void QueueToDestroy(size_t tileIndex);
	void Save();
	void Load();
};