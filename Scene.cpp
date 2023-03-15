#include "Scene.h"
#include "Prefabs.h"

#include <fileapi.h>
#include <fstream>
#include <string>
#include <algorithm>


Scene::Scene(Input* pInput)
	:
	pInput(pInput),
	pCurrentCamera(nullptr),
	current_prefab(PREFAB_Block_Dynamic),
	pPlayer(nullptr)
{
}

void Scene::Collision()
{
	// Collision


	for (auto& pObjectA : vpGameObjects)
	{
		if (dynamic_cast<Box*>(pObjectA))
			for (auto& pObjectB : vpGameObjects)
				if (pObjectB != pObjectA)
				{
					if (dynamic_cast<Box*>(pObjectB))
						dynamic_cast<Box*>(pObjectA)->m_collider.CheckCollision(&dynamic_cast<Box*>(pObjectB)->m_collider);
					else if (dynamic_cast<Ball*>(pObjectB))
						dynamic_cast<Box*>(pObjectA)->m_collider.CheckCollision(&dynamic_cast<Ball*>(pObjectB)->m_collider);
				}

		if (dynamic_cast<Ball*>(pObjectA))
			for (auto& pObjectB : vpGameObjects)
				if (pObjectB != pObjectA)
				{
					if (dynamic_cast<Box*>(pObjectB))
						dynamic_cast<Ball*>(pObjectA)->m_collider.CheckCollision(&dynamic_cast<Box*>(pObjectB)->m_collider);
					else if (dynamic_cast<Ball*>(pObjectB))
						dynamic_cast<Ball*>(pObjectA)->m_collider.CheckCollision(&dynamic_cast<Ball*>(pObjectB)->m_collider);
				}
	}

}

Camera& Scene::GetCamera()
{
	return *pCurrentCamera;
}

Input& Scene::GetInput()
{
	return *pInput;
}


void Scene::Update(float deltaTime)
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


	// Check if a player exists.
	if (!pPlayer)
	{
		bool playerExists{ false };

		for (const auto& pGameObject : vpGameObjects)
			if (dynamic_cast<Player*>(pGameObject))
				playerExists = true;

		for (const auto& pSpawnObject : vpSpawnQueue)
			if (dynamic_cast<Player*>(pSpawnObject))
				playerExists = true;

		if (!playerExists)
			QueueToSpawn(PREFAB_Player);
	}

	// Spawn.
	while (!vpSpawnQueue.empty())
	{
		vpGameObjects.push_back(vpSpawnQueue.back());
		vpSpawnQueue.pop_back();
	}

	// Assign current player
	if (!pPlayer)
	{
		for (const auto& pGameObject : vpGameObjects)
			if (dynamic_cast<Player*>(pGameObject))
			{
				pPlayer = dynamic_cast<Player*>(pGameObject);
				pPlayer->m_pInput = pInput;
			}
	}

	// Assign current camera
	if (!pCurrentCamera && pPlayer)
	{
		pCurrentCamera = &pPlayer->m_camera;
	}

	// Object delete controls
	if (pInput->CheckHeld(BTN_RMB))
	{
		game::Float2 loc{
			pCurrentCamera->ScreenLocToWorldLoc(
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

	// Object spawn controls.
	if (pInput->CheckPressed(BTN_LMB))
	{
		game::Float2 loc{
			pCurrentCamera->ScreenLocToWorldLoc(
					pInput->GetMouseLoc().x,
					pInput->GetMouseLoc().y
				)
		};
		game::Float2 locRounded{ roundf(loc.x), roundf(loc.y) };

		bool canSpawn{ true };
		for (auto& pGameObject : vpGameObjects)
			if (pGameObject->m_location == locRounded && pGameObject->m_sprite.layer < SL_Object) canSpawn = false;

		if (canSpawn)
			QueueToSpawn(current_prefab, locRounded);
	}

	if (pInput->CheckPressed(BTN_1)) current_prefab = PREFAB_Block_Wall;
	if (pInput->CheckPressed(BTN_2)) current_prefab = PREFAB_Block_Floor;
	if (pInput->CheckPressed(BTN_3)) current_prefab = PREFAB_Block_Dynamic;
	if (pInput->CheckPressed(BTN_4)) current_prefab = PREFAB_BallStatic;
	if (pInput->CheckPressed(BTN_5)) current_prefab = PREFAB_BallDynamic;
	if (pInput->CheckPressed(BTN_6)) current_prefab = PREFAB_Mushroom;
	if (pInput->CheckPressed(BTN_7)) current_prefab = PREFAB_Tree;
	if (pInput->CheckPressed(BTN_8)) current_prefab = PREFAB_Rock;
	if (pInput->CheckPressed(BTN_9)) current_prefab = PREFAB_Dirt;

	// Levels
	//if (pInput->CheckPressed(BTN_0)) Save();
	//if (pInput->CheckPressed(BTN_9)) Load();

	Collision();

	// Update
	for (auto& pGameObject : vpGameObjects)
		pGameObject->Update(deltaTime);
}   

void Scene::QueueToSpawn(int prefab, game::Float2 location)
{
	GameObject* pPrefab{ prefabList.Get(prefab) };
	if (dynamic_cast<Player*>(pPrefab))
	{
		vpSpawnQueue.push_back(new Player(*dynamic_cast<Player*>(pPrefab)));
		vpSpawnQueue.back()->m_location = location;
	}
	else if (dynamic_cast<Character*>(pPrefab))
	{
		vpSpawnQueue.push_back(new Character(*dynamic_cast<Character*>(pPrefab)));
		vpSpawnQueue.back()->m_location = location;
	}
	else if (dynamic_cast<Box*>(pPrefab))
	{
		vpSpawnQueue.push_back(new Box(*dynamic_cast<Box*>(pPrefab)));
		vpSpawnQueue.back()->m_location = location;
	}
	else if (dynamic_cast<Ball*>(pPrefab))
	{
		vpSpawnQueue.push_back(new Ball(*dynamic_cast<Ball*>(pPrefab)));
		vpSpawnQueue.back()->m_location = location;
	}
	else
	{
		vpSpawnQueue.push_back(new GameObject(*pPrefab));
		vpSpawnQueue.back()->m_location = location;
	}
}

void Scene::QueueToDestroy(size_t tileIndex)
{
	vDestroyQueue.push_back(tileIndex);
}

void Scene::Save()
{
	std::string directory{ "Data/Levels/"};
	directory += "level.csv";
	
	std::ofstream file;
	file.open(directory, std::ios_base::out);
	
	std::string buffer;
	for (const auto& pGameObject : vpGameObjects)
	{
		// Location
		buffer += std::to_string(pGameObject->m_location.x);	buffer += ',';
		buffer += std::to_string(pGameObject->m_location.y);	buffer += ',';
		// Sprite
		buffer += std::to_string(pGameObject->m_sprite.layer);		buffer += ',';
		buffer += std::to_string(pGameObject->m_sprite.texture);	buffer += ',';
		buffer += std::to_string(pGameObject->m_sprite.size.x);		buffer += ',';
		buffer += std::to_string(pGameObject->m_sprite.size.y);		buffer += ',';
		buffer += std::to_string(pGameObject->m_sprite.offset.x);	buffer += ',';
		buffer += std::to_string(pGameObject->m_sprite.offset.y);	buffer += ',';

		// Ball
		if (dynamic_cast<Ball*>(pGameObject))
		{
			Ball* pBall{ dynamic_cast<Ball*>(pGameObject) };
			
			buffer += std::to_string(pBall->m_collider.block);			buffer += ',';
			buffer += std::to_string(pBall->m_collider.dynamic);		buffer += ',';
			buffer += std::to_string(pBall->m_collider.hit_immovable);	buffer += ',';
			buffer += std::to_string(pBall->m_collider.moveBuffer.x);	buffer += ',';
			buffer += std::to_string(pBall->m_collider.moveBuffer.y);	buffer += ',';
			buffer += std::to_string(pBall->m_collider.moving);			buffer += ',';
			buffer += std::to_string(pBall->m_collider.origin.x);		buffer += ',';
			buffer += std::to_string(pBall->m_collider.origin.y);		buffer += ',';
			buffer += std::to_string(pBall->m_collider.radius);			buffer += ',';

			// Player
			if (dynamic_cast<Character*>(pGameObject))
			{
				Character* pPlayer{ dynamic_cast<Character*>(pGameObject) };

				// Speed
				buffer += std::to_string(pPlayer->m_speed); buffer += ',';
			}
		}
		// Box
		else if (dynamic_cast<Box*>(pGameObject))
		{
			Box* pTile{ dynamic_cast<Box*>(pGameObject) };

			buffer += std::to_string(pTile->m_collider.block);			buffer += ',';
			buffer += std::to_string(pTile->m_collider.dynamic);		buffer += ',';
			buffer += std::to_string(pTile->m_collider.hit_immovable);	buffer += ',';
			buffer += std::to_string(pTile->m_collider.moveBuffer.x);	buffer += ',';
			buffer += std::to_string(pTile->m_collider.moveBuffer.y);	buffer += ',';
			buffer += std::to_string(pTile->m_collider.moving);			buffer += ',';
			buffer += std::to_string(pTile->m_collider.origin.x);		buffer += ',';
			buffer += std::to_string(pTile->m_collider.origin.y);		buffer += ',';
			buffer += std::to_string(pTile->m_collider.size.x);			buffer += ',';
			buffer += std::to_string(pTile->m_collider.size.y);			buffer += ',';
		}
	}
	
	file.write(buffer.c_str(), buffer.size());
	
	file.close();
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
