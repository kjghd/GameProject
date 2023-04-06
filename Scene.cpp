#include "Scene.h"
#include "Prefabs.h"

#include "Box.h"
#include "Ball.h"
#include "Character.h"
#include "NPC.h"

#include <fileapi.h>
#include <fstream>
#include <string>
#include <algorithm>


Scene::Scene(Input* pInput)
	:
	state(SState_Run),
	pInput(pInput),
	pCurrentCamera(nullptr),
	current_prefab(PREFAB_W_Floor),
	pPlayer(nullptr),
	prefabs()
	//ui_background(new ScreenObject(*prefabs.GetScreenObject(PREFAB_S_Background))),
	//button_resume(new ScreenObject(*prefabs.GetScreenObject(PREFAB_S_Resume))),
	//button_mainMenu(new ScreenObject(*prefabs.GetScreenObject(PREFAB_S_MainMenu)))
{
	//vpScreenObjects.push_back(new ScreenObject(*prefabs.GetScreenObject(PREFAB_S_Background)));
	//vpScreenObjects.back()->SetLocation_percentage({ .5f,.5f });
	//
	//vpScreenObjects.push_back(new ScreenObject(*prefabs.GetScreenObject(PREFAB_S_Resume)));
	//vpScreenObjects.back()->SetLocation_percentage({ .5f,.4f });
	//
	//vpScreenObjects.push_back(new ScreenObject(*prefabs.GetScreenObject(PREFAB_S_MainMenu)));
	//vpScreenObjects.back()->SetLocation_percentage({ .5f,.6f });

	//ui_background->SetLocation_percentage({ .5,.5 });
	//
	//button_resume->SetLocation_percentage({ .5,.4 });
	//button_resume->SetInput(pInput);
	//
	//button_mainMenu->SetLocation_percentage({ .5,.6 });
	//button_mainMenu->SetInput(pInput);


}

Scene::~Scene()
{
	for (auto& pGameObject : vpGameObjects)
	{
		delete pGameObject;
		pGameObject = nullptr;
	}

	for (auto& pScreenObject : vpScreenObjects)
	{
		delete pScreenObject;
		pScreenObject = nullptr;
	}


	//delete ui_background;
	//ui_background = nullptr;
	//
	//delete button_resume;
	//button_resume = nullptr;
	//
	//delete button_mainMenu;
	//button_mainMenu = nullptr;
}

void Scene::Collision()
{
	// Collision


	for (auto& pObjectA : vpGameObjects)
	{
		if (dynamic_cast<Box*>(pObjectA))
		{
			for (auto& pObjectB : vpGameObjects)
				if (pObjectB != pObjectA)
				{
					if (dynamic_cast<Box*>(pObjectB))
						dynamic_cast<Box*>(pObjectA)->m_collider.CheckCollision(&dynamic_cast<Box*>(pObjectB)->m_collider);
					else if (dynamic_cast<Ball*>(pObjectB))
						dynamic_cast<Box*>(pObjectA)->m_collider.CheckCollision(&dynamic_cast<Ball*>(pObjectB)->m_collider);
				}
		}


		else if (dynamic_cast<Ball*>(pObjectA))
		{
			for (auto& pObjectB : vpGameObjects)
				if (pObjectB != pObjectA)
				{
					if (dynamic_cast<Box*>(pObjectB))
					{
						dynamic_cast<Ball*>(pObjectA)->m_collider.CheckCollision(&dynamic_cast<Box*>(pObjectB)->m_collider);

						if (dynamic_cast<Character*>(pObjectA))
							dynamic_cast<Character*>(pObjectA)->m_viewRange.CheckCollision(&dynamic_cast<Box*>(pObjectB)->m_collider);
					}
					else if (dynamic_cast<Ball*>(pObjectB))
					{
						dynamic_cast<Ball*>(pObjectA)->m_collider.CheckCollision(&dynamic_cast<Ball*>(pObjectB)->m_collider);

						if (dynamic_cast<Character*>(pObjectA))
							dynamic_cast<Character*>(pObjectA)->m_viewRange.CheckCollision(&dynamic_cast<Ball*>(pObjectB)->m_collider);
					}
				}
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
	switch (state)
	{
	case SState_Run:
	{
		DestroyObjects();

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
				QueueToSpawn(PREFAB_W_Player);
		}

		SpawnObjects();

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


		/* Player Controls */
		if (pInput->CheckPressed(BTN_ESC))
		{
			//state = SState_Pause;
		}

		// Object delete controls
		if (pInput->CheckPressed(BTN_RMB))
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
				{
					if (dynamic_cast<Character*>(pGameObject))
					{
						Character* pCharacter{ dynamic_cast<Character*>(pGameObject) };
						if (pCharacter->m_health > 0)
						{
							pCharacter->m_health = 0;
						}
						else
							QueueToDestroy(i);
					}
					else
						QueueToDestroy(i);
				}
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
				if (pGameObject->m_location == locRounded && pGameObject->m_sprite.GetRenderLayer() < SL_Object) canSpawn = false;

			if (canSpawn)
				QueueToSpawn(current_prefab, locRounded);
		}

		if (pInput->CheckPressed(BTN_1)) current_prefab = PREFAB_W_BallDynamic;
		if (pInput->CheckPressed(BTN_2)) current_prefab = PREFAB_W_Mushroom;
		if (pInput->CheckPressed(BTN_3)) current_prefab = PREFAB_W_Floor;
		if (pInput->CheckPressed(BTN_4)) current_prefab = PREFAB_W_NPC;


		Collision();

		// Update
		for (auto& pGameObject : vpGameObjects)
			pGameObject->Update(deltaTime);

		break;
	}
	case SState_Pause:
	{
		//if (pInput->CheckPressed(BTN_ESC)) state = SState_Run;
		//if (button_resume->Pressed()) state = SState_Run;
		//
		//ui_background->Update(deltaTime);
		//button_resume->Update(deltaTime);
		//button_mainMenu->Update(deltaTime);

		break;
	}
	default: break;
	}
}   

void Scene::QueueToSpawn(int prefab, game::Float2 location)
{
	WorldObject* pPrefab{ prefabs.GetWorldObject(prefab) };
	if (dynamic_cast<Player*>(pPrefab))
	{
		vpSpawnQueue.push_back(new Player(*dynamic_cast<Player*>(pPrefab)));
		vpSpawnQueue.back()->m_location = location;
	}
	else if (dynamic_cast<NPC*>(pPrefab))
	{
		vpSpawnQueue.push_back(new NPC(*dynamic_cast<NPC*>(pPrefab)));
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
		vpSpawnQueue.push_back(new WorldObject(*pPrefab));
		vpSpawnQueue.back()->m_location = location;
	}
}

void Scene::QueueToDestroy(size_t tileIndex)
{
	vDestroyQueue.push_back(tileIndex);
}

void Scene::SpawnObjects()
{
	while (!vpSpawnQueue.empty())
	{
		vpGameObjects.push_back(vpSpawnQueue.back());
		vpSpawnQueue.pop_back();
	}
}

void Scene::DestroyObjects()
{
	std::sort(vDestroyQueue.begin(), vDestroyQueue.end(), std::greater<size_t>());
	for (const auto& destroyIndex : vDestroyQueue)
	{
		delete vpGameObjects.at(destroyIndex);
		vpGameObjects.at(destroyIndex) = nullptr;
		vpGameObjects.erase(vpGameObjects.begin() + destroyIndex);
	}
	vDestroyQueue.clear();
}