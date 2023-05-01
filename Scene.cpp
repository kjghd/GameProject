#include "Scene.h"
#include "Prefabs.h"

#include "Box.h"
#include "Ball.h"
#include "Character.h"
#include "NPC.h"

#include <algorithm>


Scene::Scene(Input* pInput, bool show)
	:
	show(show),
	active(false),
	pInput(pInput),
	pCurrentCamera(nullptr),
	defaultCamera(nullptr)
{
}

Scene::Scene(const Scene& scene)
	:
	show(scene.show),
	active(scene.active),
	pInput(scene.pInput),
	pCurrentCamera(nullptr),
	vpSpawnQueue(),
	defaultCamera(nullptr)
{
	for (const auto& pObject : scene.vpGameObjects)
	{
		if (pObject != scene.pCursor)
		{
			QueueToSpawn(pObject);
		}
	}
	for (const auto& pObject : scene.vpSpawnQueue)
	{
		if (pObject != scene.pCursor)
		{
			QueueToSpawn(pObject);
		}
	}
}

Scene::~Scene()
{
	for (auto& pGameObject : vpGameObjects)
	{
		delete pGameObject;
		pGameObject = nullptr;
	}
}


void Scene::Initialise()
{
	if (!pCursor)
	{
		vpGameObjects.push_back(new ScreenObject(*dynamic_cast<ScreenObject*>(PrefabList::Get(PREFAB_Cursor))));
		pCursor = dynamic_cast<ScreenObject*>(vpGameObjects.back());
	}

	SpawnObjects();
}


void Scene::Activate(Input* pInp)
{
	active = true;
	pInput = pInp;
}
void Scene::Dectivate()
{
	active = false;
	pInput = nullptr;
}
bool Scene::CheckActive()
{
	return active;
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

Camera* Scene::GetCamera()
{
	return pCurrentCamera ? pCurrentCamera : &defaultCamera;
}

Input& Scene::GetInput()
{
	return *pInput;
}



SceneMessage Scene::Update(float deltaTime)
{
	if (!active && show)
	{
		pCursor->SetLocation_percentage({ 100.f,100.f });
		for (auto& pGameObject : vpGameObjects)
			pGameObject->Update_SpriteOnly();
	}
	else if (active)
	{
		SceneMessage msg{ SMID_Null, 0 };

		DestroyObjects();
		SpawnObjects();

		if (pInput->CheckPressed(BTN_ESC))
		{
			msg.id = SMID_Pop;
		}


		// Cursor
		game::float2 loc_px{ pInput->GetMouseLoc().x, pInput->GetMouseLoc().y };
		pCursor->SetLocaion_px(loc_px);

		// Update
		for (auto& pGameObject : vpGameObjects)
			pGameObject->Update(deltaTime);

		return msg;
	}
}   

void Scene::QueueToSpawn(int prefab, game::float2 location)
{
	GameObject* pPrefab{ PrefabList::Get(prefab) };

	if (dynamic_cast<WorldObject*>(pPrefab))
	{
		if (dynamic_cast<Box*>(pPrefab))
		{
			vpSpawnQueue.push_back(new Box(*dynamic_cast<Box*>(pPrefab)));
			vpSpawnQueue.back()->m_location = location;
		}
		else if (dynamic_cast<Ball*>(pPrefab))
		{
			if (dynamic_cast<Character*>(pPrefab))
			{
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
				else
				{
					vpSpawnQueue.push_back(new Character(*dynamic_cast<Character*>(pPrefab)));
					vpSpawnQueue.back()->m_location = location;
				}
			}
			else
			{
				vpSpawnQueue.push_back(new Ball(*dynamic_cast<Ball*>(pPrefab)));
				vpSpawnQueue.back()->m_location = location;
			}
		}
		else
		{
			vpSpawnQueue.push_back(new WorldObject(*dynamic_cast<WorldObject*>(pPrefab)));
			vpSpawnQueue.back()->m_location = location;
		}

	}
	else if (dynamic_cast<ScreenObject*>(pPrefab))
	{
		if (dynamic_cast<SO_Button*>(pPrefab))
		{
			vpSpawnQueue.push_back(new SO_Button(*dynamic_cast<SO_Button*>(pPrefab)));
			vpSpawnQueue.back()->m_location = location;
		}
		else
		{
			vpSpawnQueue.push_back(new ScreenObject(*dynamic_cast<ScreenObject*>(pPrefab)));
			vpSpawnQueue.back()->m_location = location;
		}
	}
	else
	{
		vpSpawnQueue.push_back(new GameObject(*pPrefab));
		vpSpawnQueue.back()->m_location = location;
	}
}

void Scene::QueueToSpawn(GameObject* pObject)
{
	if (dynamic_cast<WorldObject*>(pObject))
	{
		if (dynamic_cast<Box*>(pObject))
		{
			vpSpawnQueue.push_back(new Box(*dynamic_cast<Box*>(pObject)));
		}
		else if (dynamic_cast<Ball*>(pObject))
		{
			if (dynamic_cast<Character*>(pObject))
			{
				if (dynamic_cast<Player*>(pObject))
				{
					vpSpawnQueue.push_back(new Player(*dynamic_cast<Player*>(pObject)));
				}
				else if (dynamic_cast<NPC*>(pObject))
				{
					vpSpawnQueue.push_back(new NPC(*dynamic_cast<NPC*>(pObject)));
				}
				else
				{
					vpSpawnQueue.push_back(new Character(*dynamic_cast<Character*>(pObject)));
				}
			}
			else
			{
				vpSpawnQueue.push_back(new Ball(*dynamic_cast<Ball*>(pObject)));
			}
		}
		else
		{
			vpSpawnQueue.push_back(new WorldObject(*dynamic_cast<WorldObject*>(pObject)));
		}

	}
	else if (dynamic_cast<ScreenObject*>(pObject))
	{
		if (dynamic_cast<SO_Button*>(pObject))
		{
			vpSpawnQueue.push_back(new SO_Button(*dynamic_cast<SO_Button*>(pObject)));
		}
		else
		{
			vpSpawnQueue.push_back(new ScreenObject(*dynamic_cast<ScreenObject*>(pObject)));
		}
	}
	else
	{
		vpSpawnQueue.push_back(new GameObject(*pObject));
	}

	vpSpawnQueue.back()->m_location = pObject->m_location;
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

		//if (dynamic_cast<Player*>(vpGameObjects.back()))
		//	pPlayer = dynamic_cast<Player*>(vpGameObjects.back());
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

std::string Scene::Serialise()
{
	DestroyObjects();
	SpawnObjects();

	std::string str;

	// Show
	str += std::to_string(show);

	// GameObjects
	for (const auto& pGameObject : vpGameObjects)
	{
		
	}

	// Prefabs




	std::vector<GameObject*> vpGameObjects;

	int current_prefab;
	PrefabList prefabs;

	Player* pPlayer;
	ScreenObject* pCursor;
	GameObject* pCursorBox;

	return str;
}




void Scene_World::Initialise()
{
	if (!pPlayer)
	{
		for (const auto& pObject : vpSpawnQueue)
			if (dynamic_cast<Player*>(pObject))
			{
				pPlayer = dynamic_cast<Player*>(pObject);
				pPlayer->m_pInput = pInput;
				pCurrentCamera = &pPlayer->m_camera;
			}

		if (!pPlayer)
		{
			vpGameObjects.push_back(new Player(*dynamic_cast<Player*>(PrefabList::Get(PREFAB_Player))));
			pPlayer = dynamic_cast<Player*>(vpGameObjects.back());
			pPlayer->m_pInput = pInput;
			pCurrentCamera = &pPlayer->m_camera;
		}
	}

	if (!pCursor)
	{
		vpGameObjects.push_back(new ScreenObject(*dynamic_cast<ScreenObject*>(PrefabList::Get(PREFAB_Cursor))));
		pCursor = dynamic_cast<ScreenObject*>(vpGameObjects.back());
	}

	if (!pCursorBox)
	{
		vpGameObjects.push_back(new WorldObject(*dynamic_cast<WorldObject*>(PrefabList::Get(PREFAB_Selection))));
		pCursorBox = dynamic_cast<WorldObject*>(vpGameObjects.back());
	}

	SpawnObjects();

}

Scene_World::Scene_World(Input* pInput, bool show)
	:
	Scene(pInput, show),
	current_prefab(PREFAB_Floor_ConcreteA),
	pPlayer(nullptr)
{
}

Scene_World::Scene_World(const Scene_World& scene)
	:
	Scene(scene),
	current_prefab(PREFAB_Floor_ConcreteA),
	pPlayer(nullptr)
{
}


SceneMessage Scene_World::Update(float deltaTime)
{
	if (!active && show)
	{
		for (auto& pGameObject : vpGameObjects)
			pGameObject->Update_SpriteOnly();
	}
	else if (active)
	{
		SceneMessage msg;

		DestroyObjects();
		SpawnObjects();

		if (pPlayer)
		{
			if (!pPlayer->m_pInput) pPlayer->m_pInput = pInput;
			if (!pCurrentCamera) pCurrentCamera = &pPlayer->m_camera;
		}

		/* Player Controls */
		if (pInput->CheckPressed(BTN_ESC))
		{
			msg.id = SMID_New;
			msg.indexPrefabs = SCENE_Pause;
		}

		// Object delete controls
		if (pInput->CheckPressed(BTN_RMB))
		{
			game::float2 loc{
				pCurrentCamera->ScreenLocToWorldLoc(
						pInput->GetMouseLoc().x,
						pInput->GetMouseLoc().y
					)
			};
			game::float2 locRounded{ roundf(loc.x), roundf(loc.y) };

			size_t i{};
			for (auto& pGameObject : vpGameObjects)
			{
				if (pGameObject != pPlayer && pGameObject != pCursorBox)
				{
					game::float2 tileLocRounded{
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

			game::float2 loc{
				pCurrentCamera->ScreenLocToWorldLoc(
				pInput->GetMouseLoc().x,
				pInput->GetMouseLoc().y
				)
			};
			game::float2 locRounded{ roundf(loc.x), roundf(loc.y) };
			pCursorBox->m_location = locRounded;
			pCursorBox->Update(deltaTime);

			// Object spawn controls.
			if (pInput->CheckPressed(BTN_LMB))
			{
				game::float2 loc{
					pCurrentCamera->ScreenLocToWorldLoc(
							pInput->GetMouseLoc().x,
							pInput->GetMouseLoc().y
						)
				};
				game::float2 locRounded{ roundf(loc.x), roundf(loc.y) };

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
		game::float2 loc_px{ pInput->GetMouseLoc().x, pInput->GetMouseLoc().y };
		pCursor->SetLocaion_px(loc_px);

		// Update
		for (auto& pGameObject : vpGameObjects)
			pGameObject->Update(deltaTime);

		return msg;
	}
}


void Scene_World::SpawnObjects()
{
	while (!vpSpawnQueue.empty())
	{
		vpGameObjects.push_back(vpSpawnQueue.back());
		vpSpawnQueue.pop_back();

		if (dynamic_cast<Player*>(vpGameObjects.back()))
			pPlayer = dynamic_cast<Player*>(vpGameObjects.back());
	}
}