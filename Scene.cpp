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

void Scene::Update(Input* pInput, float deltaTime)
{
	// Spawn.
	while (!vSpawnQueue.empty())
	{
		vTiles.push_back(vSpawnQueue.top());
		vSpawnQueue.pop();
	}

	// Camera controls.
	if (pInput->CheckHeld(BTN_Q)) camera.DecreaseZoom(.1f);
	if (pInput->CheckHeld(BTN_E)) camera.IncreaseZoom(.1f);

	// Tile spawn controls.
	if (pInput->CheckHeld(BTN_LMB))
	{
		game::Float2 loc{
			camera.ScreenLocToWorldLoc(
					pInput->GetMouseLoc().x,
					pInput->GetMouseLoc().y
				)
		};
		game::Float2 locRounded{ roundf(loc.x), roundf(loc.y) };

		bool canSpawn{ true };
		for (auto& tile : vTiles)
			if (tile.m_location == locRounded) canSpawn = false;

		if (canSpawn)
			QueueToSpawn(current_prefab, locRounded);
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

	// Update
	for (auto& tile : vTiles)
		tile.m_collider.Update();

	player.Update(deltaTime);
	player.m_collider.Update();

	game::Float2 playerCenter{
		player.m_location.x + player.m_sprite.offset.x,
		player.m_location.y + player.m_sprite.offset.y
	};
	camera.MoveTo(playerCenter, deltaTime);
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
		// Tile
		buffer += std::to_string(tile.m_location.x);		buffer += ',';
		buffer += std::to_string(tile.m_location.y);		buffer += ',';
		// Tile sprite
		buffer += std::to_string(tile.m_sprite.layer);		buffer += ',';
		buffer += std::to_string(tile.m_sprite.texture);	buffer += ',';
		buffer += std::to_string(tile.m_sprite.size.x);		buffer += ',';
		buffer += std::to_string(tile.m_sprite.size.y);		buffer += ',';
		buffer += std::to_string(tile.m_sprite.offset.x);	buffer += ',';
		buffer += std::to_string(tile.m_sprite.offset.y);	buffer += ',';
		// Tile collider
		buffer += std::to_string(tile.m_collider.size.x);	buffer += ',';
		buffer += std::to_string(tile.m_collider.size.y);	buffer += ',';
		buffer += std::to_string(tile.m_collider.dynamic);	buffer += ',';
		buffer += std::to_string(tile.m_collider.block);	buffer += '\n';
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
		// Tile
		game::Float2 location{
			std::stoi(values.at(0)),
			std::stoi(values.at(1))
		};
		// Tile sprite
		int sprite_layer{ std::stoi(values.at(2)) };
		int sprite_texture{ std::stoi(values.at(3)) };
		game::Float2 sprite_size{
			std::stof(values.at(4)),
			std::stof(values.at(5))
		};
		game::Float2 sprite_offset{
			std::stof(values.at(6)),
			std::stof(values.at(7))
		};
		// Tile collider
		game::Float2 collider_size{
			std::stof(values.at(8)),
			std::stof(values.at(9))
		};
		bool collider_dynamic{ std::stoi(values.at(10)) ? true : false };
		bool collider_block{ std::stoi(values.at(11)) ? true : false };

		// Add tile
		vTiles.push_back(
			Tile(
				location,
				sprite_layer, sprite_texture, sprite_size, sprite_offset,
				collider_size, collider_dynamic, collider_block
			)
		);
	}

	file.close();
}