#pragma once
#include "DataTypes.h"
#include "WorldObject.h"
#include "Input.h"

#include <vector>
#include <stack>

class Scene
{
	const char* name;
	Camera camera;
	std::stack<Tile> vSpawnQueue;

public:
	std::vector<Tile> vTiles;
	int current_prefab;

	Player player;

	Scene();

	Camera& GetCamera();

	void Update(Input* pInput, double deltaTime);

	void QueueToSpawn(int prefab, game::Float2 location = {0,0});

	void Save();
	void Load();
};