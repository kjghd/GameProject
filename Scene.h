#pragma once

#include "DataTypes.h"
#include "Camera.h"
#include "Player.h"
#include "Tile.h"
#include "Input.h"

#include <vector>
#include <stack>

class Scene
{
	const char* name;
	Camera camera;
	std::stack<Tile> vSpawnQueue;
	std::vector<size_t> vDestroyQueue;

public:
	std::vector<Tile> vTiles;
	int current_prefab;

	Player player;

	Scene();

	Camera& GetCamera();

	void Update(Input* pInput, float deltaTime);

	void QueueToSpawn(int prefab, game::Float2 location = {0,0});
	void QueueToDestroy(size_t tileIndex);
	void Save();
	void Load();
};