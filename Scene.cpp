#include "Scene.h"
#include "Prefabs.h"
#include <fileapi.h>
#include <fstream>
#include <string>

Scene::Scene()
	:
	camera(Camera()),
	current_prefab(PREFAB_Block_Dynamic),
	player(Player()),
	name("cool level")
{
	//Load();
}

Camera& Scene::GetCamera()
{
	return camera;
}

void Scene::Update(Input* pInput, double deltaTime)
{
	// Spawn.
	while (!vSpawnQueue.empty())
	{
		vTiles.push_back(vSpawnQueue.top());
		vSpawnQueue.pop();
	}

	// Camera controls.
	if (pInput->CheckHeld(BTN_Q)) camera.SetZoom(128);
	if (pInput->CheckHeld(BTN_E)) camera.SetZoom(64);

	// Tile spawn controls.
	if (pInput->CheckPressed(BTN_LMB))
	{
		game::Float2 loc{
			camera.ScreenLocToWorldLoc(
					pInput->GetMouseLoc().x,
					pInput->GetMouseLoc().y
				)
		};

		QueueToSpawn(current_prefab, { roundf(loc.x), roundf(loc.y) });
	}
	if (pInput->CheckPressed(BTN_1)) current_prefab = PREFAB_Block_Wall;
	if (pInput->CheckPressed(BTN_2)) current_prefab = PREFAB_Block_Floor;
	if (pInput->CheckPressed(BTN_3)) current_prefab = PREFAB_Block_Dynamic;

	// Levels
	if (pInput->CheckPressed(BTN_0)) Save();
	if (pInput->CheckPressed(BTN_9)) Load();

	// Player
	if (pInput->CheckHeld(BTN_W)) player.Move({ 0, 1 }, deltaTime);
	if (pInput->CheckHeld(BTN_S)) player.Move({ 0,-1 }, deltaTime);
	if (pInput->CheckHeld(BTN_A)) player.Move({ -1, 0 }, deltaTime);
	if (pInput->CheckHeld(BTN_D)) player.Move({ 1, 0 }, deltaTime);

	// Collision
	for (auto& tile : vTiles)
	{
		for (auto& tileB : vTiles)
			tile.m_collider.CheckCollision(&tileB.m_collider);

		tile.m_collider.CheckCollision(&player.m_collider);
		player.m_collider.CheckCollision(&tile.m_collider);
	}
	player.Update(deltaTime);

	camera.MoveTo(player.m_location, deltaTime);
}   

void Scene::QueueToSpawn(int prefab, game::Float2 location)
{
	vSpawnQueue.push(*prefabList.Get(prefab));
	vSpawnQueue.top().m_location = location;
}

void Scene::Save()
{
	std::string directory{ "Data/Levels/"};
	directory += name;
	directory += ".csv";

	std::ofstream file;
	file.open(directory, std::ios_base::out);

	std::string buffer;
	for (const auto& tile : vTiles)
	{
		buffer += std::to_string(tile.m_texture);		buffer += ',';
		buffer += std::to_string(tile.m_location.x);	buffer += ',';
		buffer += std::to_string(tile.m_location.y);	buffer += ',';
		//buffer += std::to_string(tile.m_block);			buffer += '\n';
	}

	file.write(buffer.c_str(), buffer.size());

	file.close();
}

void Scene::Load()
{
	vTiles.clear();

	player.m_location = { 0,0 };

	std::string directory{ "Data/Levels/" };
	directory += "example";
	directory += ".csv";

	std::ifstream file;
	file.open(directory, std::ios_base::out);

	while (file.peek() != EOF)
	{
		// Get line
		std::string buffer;
		std::getline(file, buffer);

		// Seperate values
		std::vector<std::string> values;
		values.push_back("");
		for (const auto& c : buffer)
		{
			if (c != ',')
				values.back() += c;
			else
				values.push_back("");
		}

		// Cast
		int texture{ std::stoi(values.at(0)) };
		game::Float2 location{
			std::stof(values.at(1)),
			std::stof(values.at(2))
		};
		bool block{ std::stoi(values.at(3)) ? true : false};

		// Add block
		vTiles.push_back(Tile(texture, location, block));
	}

	file.close();
}