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
	current_prefab(PREFAB_Floor_ConcreteA),
	pPlayer(nullptr),
	prefabs()
{
	vpGameObjects.push_back(new Player(*dynamic_cast<Player*>(prefabs.GetGameObject(PREFAB_Player))));
	pPlayer = dynamic_cast<Player*>(vpGameObjects.back());
	pPlayer->m_pInput = pInput;
	pCurrentCamera = &pPlayer->m_camera;

	vpGameObjects.push_back(new ScreenObject(*dynamic_cast<ScreenObject*>(prefabs.GetGameObject(PREFAB_Cursor))));
	pCursor = dynamic_cast<ScreenObject*>(vpGameObjects.back());

	vpGameObjects.push_back(new WorldObject(*dynamic_cast<WorldObject*>(prefabs.GetGameObject(PREFAB_Selection))));
	pCursorBox = dynamic_cast<WorldObject*>(vpGameObjects.back());

	//vpGameObjects.push_back(new ScreenObject(*dynamic_cast<ScreenObject*>(prefabs.GetGameObject(PREFAB_Cursor))));
	//pCursor = dynamic_cast<ScreenObject*>(vpGameObjects.back());

	//vpGameObjects.push_back(new ScreenObject(*dynamic_cast<ScreenObject*>(prefabs.GetGameObject(PREFAB_Background))));
	//dynamic_cast<ScreenObject*>(vpGameObjects.back())->SetLocation_percentage({ .5f,.5f });
	//
	//vpGameObjects.push_back(new SO_Button(*dynamic_cast<SO_Button*>(prefabs.GetGameObject(PREFAB_Resume))));
	//pResume = dynamic_cast<SO_Button*>(vpGameObjects.back());
	//pResume->SetLocation_percentage({ .5f,.4f });
	//pResume->SetInput(pInput);
	//
	//vpGameObjects.push_back(new SO_Button(*dynamic_cast<SO_Button*>(prefabs.GetGameObject(PREFAB_MainMenu))));
	//pMainMenu = dynamic_cast<SO_Button*>(vpGameObjects.back());
	//pMainMenu->SetLocation_percentage({ .5f,.6f });
	//pMainMenu->SetInput(pInput);
}

Scene::~Scene()
{
	for (auto& pGameObject : vpGameObjects)
	{
		delete pGameObject;
		pGameObject = nullptr;
	}
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

Player& Scene::GetPlayer()
{
	return *pPlayer;
}



void Scene::Update(float deltaTime)
{
	switch (state)
	{
	case SState_Run:
	{
		DestroyObjects();
		SpawnObjects();

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
				if (pGameObject != pPlayer && pGameObject != pCursorBox)
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
				}
				++i;
			}

		}

		if (pInput->CheckHeld(BTN_SHIFT))
		{
			pCursorBox->m_sprite.visible = true;

			game::Float2 loc{
				pCurrentCamera->ScreenLocToWorldLoc(
				pInput->GetMouseLoc().x,
				pInput->GetMouseLoc().y
				)
			};
			game::Float2 locRounded{ roundf(loc.x), roundf(loc.y) };
			pCursorBox->m_location = locRounded;
			pCursorBox->Update(deltaTime);

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
					if (pGameObject->m_location == locRounded && pGameObject->m_sprite.GetRenderLayer() < SL_Object && pGameObject != pCursorBox) canSpawn = false;

				if (canSpawn)
					QueueToSpawn(current_prefab, locRounded);
			}
		}
		else
			pCursorBox->m_sprite.visible = false;
		if (pInput->CheckPressed(BTN_2)) current_prefab = PREFAB_Mushroom;
		if (pInput->CheckPressed(BTN_3)) current_prefab = PREFAB_Floor_ConcreteA;
		if (pInput->CheckPressed(BTN_4)) current_prefab = PREFAB_NPC;

		// Pause
		//if (pInput->CheckPressed(BTN_ESC)) state = SState_Pause;

		Collision();

		// Cursor




		game::Float2 loc_px{ pInput->GetMouseLoc().x, pInput->GetMouseLoc().y };
		pCursor->SetLocaion_px(loc_px);

		// Update
		for (auto& pGameObject : vpGameObjects)
			pGameObject->Update(deltaTime);

		break;
	}
	case SState_Pause:
	{
		// Resume
		if (pInput->CheckPressed(BTN_ESC) || pResume->Pressed()) state = SState_Run;

		if (pMainMenu->Pressed()) {}

		// Update
		for (auto& pGameObject : vpGameObjects)
			pGameObject->Update(deltaTime);

		break;
	}
	default: break;
	}
}   

void Scene::QueueToSpawn(int prefab, game::Float2 location)
{
	GameObject* pPrefab{ prefabs.GetGameObject(prefab) };
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
	else if (dynamic_cast<WorldObject*>(pPrefab))
	{
		vpSpawnQueue.push_back(new WorldObject(*dynamic_cast<WorldObject*>(pPrefab)));
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