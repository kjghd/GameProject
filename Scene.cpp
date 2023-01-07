#include "Scene.h"
#include "Prefabs.h"

#include <fileapi.h>
#include <fstream>
#include <string>
#include <algorithm>


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
	// Destroy.
	std::sort(vDestroyQueue.begin(), vDestroyQueue.end(), std::greater<size_t>());
	for (const auto& destroyIndex : vDestroyQueue)
	{
		delete vpGameObjects.at(destroyIndex);
		vpGameObjects.at(destroyIndex) = nullptr;
		vpGameObjects.erase(vpGameObjects.begin() + destroyIndex);
	}
	vDestroyQueue.clear();

	// Spawn.
	while (!vpSpawnQueue.empty())
	{
		vpGameObjects.push_back(vpSpawnQueue.top());
		vpSpawnQueue.pop();
	}

	// Camera controls.
	if (pInput->CheckHeld(BTN_E)) camera.DecreaseZoom(.1f);
	if (pInput->CheckHeld(BTN_Q)) camera.IncreaseZoom(.1f);
	
	// Tile delete controls
	if (pInput->CheckHeld(BTN_RMB))
	{
		game::Float2 loc{
			camera.ScreenLocToWorldLoc(
					pInput->GetMouseLoc().x,
					pInput->GetMouseLoc().y
				)
		};
		game::Float2 locRounded{ roundf(loc.x), roundf(loc.y) };

		size_t i{};
		for (auto& pGameObject : vpGameObjects)
		{
			game::Float2 tileLocRounded{
				roundf(pGameObject->m_location.x),
				roundf(pGameObject->m_location.y)
			};
			if (tileLocRounded == locRounded)
				QueueToDestroy(i);
			++i;
		}

	}

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
		for (auto& pGameObject : vpGameObjects)
			if (pGameObject->m_location == locRounded && pGameObject->m_sprite.layer < SL_Floor) canSpawn = false;

		if (canSpawn)
			QueueToSpawn(current_prefab, locRounded);
	}

	if (pInput->CheckPressed(BTN_1)) current_prefab = PREFAB_Block_Wall;
	if (pInput->CheckPressed(BTN_2)) current_prefab = PREFAB_Block_Floor;
	if (pInput->CheckPressed(BTN_3)) current_prefab = PREFAB_Block_Dynamic;
	if (pInput->CheckPressed(BTN_4)) current_prefab = PREFAB_BallStatic;
	if (pInput->CheckPressed(BTN_5)) current_prefab = PREFAB_BallDynamic;


	// Levels
	//if (pInput->CheckPressed(BTN_0)) Save();
	//if (pInput->CheckPressed(BTN_9)) Load();

	// Player
	if (pInput->CheckHeld(BTN_W)) player.Move({ 0, 1 }, deltaTime);
	if (pInput->CheckHeld(BTN_S)) player.Move({ 0,-1 }, deltaTime);
	if (pInput->CheckHeld(BTN_A)) player.Move({ -1, 0 }, deltaTime);
	if (pInput->CheckHeld(BTN_D)) player.Move({ 1, 0 }, deltaTime);

	// Collision
	for (auto& pObjectA : vpGameObjects)
	{
		if (dynamic_cast<Tile*>(pObjectA))
		{
			for (auto& pObjectB : vpGameObjects)
			{
				if (pObjectB != pObjectA)
				{
					if (dynamic_cast<Tile*>(pObjectB))
						dynamic_cast<Tile*>(pObjectA)->m_collider.CheckCollision(&dynamic_cast<Tile*>(pObjectB)->m_collider);
					else if (dynamic_cast<Ball*>(pObjectB))
						dynamic_cast<Tile*>(pObjectA)->m_collider.CheckCollision(&dynamic_cast<Ball*>(pObjectB)->m_collider);
				}
			}
			dynamic_cast<Tile*>(pObjectA)->m_collider.CheckCollision(&player.m_collider);
			player.m_collider.CheckCollision(&dynamic_cast<Tile*>(pObjectA)->m_collider);
		}
		if (dynamic_cast<Ball*>(pObjectA))
		{
			for (auto& pObjectB : vpGameObjects)
			{
				if (pObjectB != pObjectA)
				{
					if (dynamic_cast<Tile*>(pObjectB))
						dynamic_cast<Ball*>(pObjectA)->m_collider.CheckCollision(&dynamic_cast<Tile*>(pObjectB)->m_collider);
					else if (dynamic_cast<Ball*>(pObjectB))
						dynamic_cast<Ball*>(pObjectA)->m_collider.CheckCollision(&dynamic_cast<Ball*>(pObjectB)->m_collider);
				}
			}
			dynamic_cast<Ball*>(pObjectA)->m_collider.CheckCollision(&player.m_collider);
			player.m_collider.CheckCollision(&dynamic_cast<Ball*>(pObjectA)->m_collider);
		}
	}

	// Update
	for (auto& pGameObject : vpGameObjects)
	{
		if (dynamic_cast<Tile*>(pGameObject))
			dynamic_cast<Tile*>(pGameObject)->m_collider.Update();
		else if (dynamic_cast<Ball*>(pGameObject))
			dynamic_cast<Ball*>(pGameObject)->m_collider.Update();
	}

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
	GameObject* pPrefab{ prefabList.Get(prefab) };
	if (dynamic_cast<Tile*>(pPrefab))
	{
		vpSpawnQueue.push(new Tile(*dynamic_cast<Tile*>(pPrefab)));
		vpSpawnQueue.top()->m_location = location;
	}
	else if (dynamic_cast<Ball*>(pPrefab))
	{
		vpSpawnQueue.push(new Ball(*dynamic_cast<Ball*>(pPrefab)));
		vpSpawnQueue.top()->m_location = location;
	}
}

void Scene::QueueToDestroy(size_t tileIndex)
{
	vDestroyQueue.push_back(tileIndex);
}

void Scene::Save()
{
	//std::string directory{ "Data/Levels/"};
	//directory += name;
	//directory += ".csv";
	//
	//std::ofstream file;
	//file.open(directory, std::ios_base::out);
	//
	//std::string buffer;
	//for (const auto& tile : vTiles)
	//{
	//	// Tile
	//	buffer += std::to_string(tile.m_location.x);		buffer += ',';
	//	buffer += std::to_string(tile.m_location.y);		buffer += ',';
	//	// Tile sprite
	//	buffer += std::to_string(tile.m_sprite.layer);		buffer += ',';
	//	buffer += std::to_string(tile.m_sprite.texture);	buffer += ',';
	//	buffer += std::to_string(tile.m_sprite.size.x);		buffer += ',';
	//	buffer += std::to_string(tile.m_sprite.size.y);		buffer += ',';
	//	buffer += std::to_string(tile.m_sprite.offset.x);	buffer += ',';
	//	buffer += std::to_string(tile.m_sprite.offset.y);	buffer += ',';
	//	// Tile collider
	//	buffer += std::to_string(tile.m_collider.size.x);	buffer += ',';
	//	buffer += std::to_string(tile.m_collider.size.y);	buffer += ',';
	//	buffer += std::to_string(tile.m_collider.dynamic);	buffer += ',';
	//	buffer += std::to_string(tile.m_collider.block);	buffer += '\n';
	//}
	//
	//file.write(buffer.c_str(), buffer.size());
	//
	//file.close();
}

void Scene::Load()
{
	//vTiles.clear();
	//
	//player.m_location = { 0,0 };
	//
	//std::string directory{ "Data/Levels/" };
	//directory += "example";
	//directory += ".csv";
	//
	//std::ifstream file;
	//file.open(directory, std::ios_base::out);
	//
	//while (file.peek() != EOF)
	//{
	//	// Get line
	//	std::string buffer;
	//	std::getline(file, buffer);
	//
	//	// Seperate values
	//	std::vector<std::string> values;
	//	values.push_back("");
	//	for (const auto& c : buffer)
	//	{
	//		if (c != ',')
	//			values.back() += c;
	//		else
	//			values.push_back("");
	//	}
	//
	//	// Cast
	//	// Tile
	//	game::Float2 location{
	//		std::stoi(values.at(0)),
	//		std::stoi(values.at(1))
	//	};
	//	// Tile sprite
	//	int sprite_layer{ std::stoi(values.at(2)) };
	//	int sprite_texture{ std::stoi(values.at(3)) };
	//	game::Float2 sprite_size{
	//		std::stof(values.at(4)),
	//		std::stof(values.at(5))
	//	};
	//	game::Float2 sprite_offset{
	//		std::stof(values.at(6)),
	//		std::stof(values.at(7))
	//	};
	//	// Tile collider
	//	game::Float2 collider_size{
	//		std::stof(values.at(8)),
	//		std::stof(values.at(9))
	//	};
	//	bool collider_dynamic{ std::stoi(values.at(10)) ? true : false };
	//	bool collider_block{ std::stoi(values.at(11)) ? true : false };
	//
	//	// Add tile
	//	vTiles.push_back(
	//		Tile(
	//			location,
	//			sprite_layer, sprite_texture, sprite_size, sprite_offset,
	//			collider_size, collider_dynamic, collider_block
	//		)
	//	);
	//}
	//
	//file.close();
}
