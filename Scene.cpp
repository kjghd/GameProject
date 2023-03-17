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
	current_prefab(PREFAB_BallDynamic),
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

	if (pInput->CheckPressed(BTN_1)) current_prefab = PREFAB_BallDynamic;
	if (pInput->CheckPressed(BTN_2)) current_prefab = PREFAB_Mushroom;
	if (pInput->CheckPressed(BTN_3)) current_prefab = PREFAB_Animation;

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
