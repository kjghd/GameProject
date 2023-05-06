#include "Scene.h"

#include "Input.h"
#include "Prefabs.h"
#include "GameObject.h"
#include "WorldObject.h"
#include "Box.h"
#include "Ball.h"
#include "Character.h"
#include "NPC.h"
#include "Player.h"
#include "ScreenObject.h"
#include "SO_Button.h"

#include <algorithm>


Scene::Scene(bool show)
	:
	isVisible(show),
	canUpdate(false)
{
}

Scene::Scene(const Scene& scene)
	:
	isVisible(scene.isVisible),
	canUpdate(scene.canUpdate),
	vpSpawnQueue()
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
	for (auto& pGameObject : vpSpawnQueue)
	{
		delete pGameObject;
		pGameObject = nullptr;
	}
}


void Scene::Initialise()
{
	SpawnObjects();
}


void Scene::Activate()
{
	canUpdate = true;
	isVisible = true;
}
void Scene::Deactivate(bool show)
{
	canUpdate = false;
	//isVisible = show;
}
bool Scene::CheckActive()
{
	return canUpdate;
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


SceneMessage Scene::Update(float deltaTime)
{
	if (!canUpdate && isVisible)
	{
		pCursor->SetLocation_percentage({ 100.f,100.f });
		for (auto& pGameObject : vpGameObjects)
			pGameObject->Update_SpriteOnly();
	}
	else if (canUpdate)
	{
		SceneMessage msg{ SMID_Null, 0 };

		DestroyObjects();
		SpawnObjects();

		if (Input::CheckPressed(BTN_ESC))
		{
			msg.id = SMID_Pop;
		}


		// Cursor
		game::float2 loc_px{ Input::GetMouseLoc().x, Input::GetMouseLoc().y };
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
	str += std::to_string(isVisible);

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

void Scene::SetCursor(ScreenObject* pC)
{
	pCursor = pC;
}



void Scene_World::Initialise()
{
	if (!pPlayer)
	{
		for (const auto& pObject : vpSpawnQueue)
			if (dynamic_cast<Player*>(pObject))
			{
				pPlayer = dynamic_cast<Player*>(pObject);
				pCurrentCamera = &pPlayer->m_camera;
			}

		if (!pPlayer)
		{
			vpGameObjects.push_back(new Player(*dynamic_cast<Player*>(PrefabList::Get(PREFAB_Player))));
			pPlayer = dynamic_cast<Player*>(vpGameObjects.back());
			pCurrentCamera = &pPlayer->m_camera;
		}
	}

	if (!pCursorBox)
	{
		vpGameObjects.push_back(new WorldObject(*dynamic_cast<WorldObject*>(PrefabList::Get(PREFAB_Selection))));
		pCursorBox = dynamic_cast<WorldObject*>(vpGameObjects.back());
	}

	SpawnObjects();

}

Scene_World::Scene_World(bool show)
	:
	Scene(show),
	current_prefab(PREFAB_Floor_ConcreteA),
	pPlayer(nullptr),
	pCurrentCamera(nullptr)
{
}

Scene_World::Scene_World(const Scene_World& scene)
	:
	Scene(scene),
	current_prefab(PREFAB_Floor_ConcreteA),
	pPlayer(nullptr),
	pCurrentCamera(nullptr)
{
}


SceneMessage Scene_World::Update(float deltaTime)
{
	if (!canUpdate && isVisible)
	{
		for (auto& pGameObject : vpGameObjects)
			pGameObject->Update_SpriteOnly();
	}
	else if (canUpdate)
	{
		SceneMessage msg;

		DestroyObjects();
		SpawnObjects();

		if (pPlayer)
		{
			if (!pCurrentCamera) pCurrentCamera = &pPlayer->m_camera;
		}

		if (Input::CheckPressed(BTN_ESC))
		{
			msg.id = SMID_New;
			msg.indexPrefabs = SCENE_Pause;
		}

		if (Input::CheckPressed(BTN_RMB))
		{
			game::float2 loc{
				pCurrentCamera->ScreenLocToWorldLoc(
						Input::GetMouseLoc().x,
						Input::GetMouseLoc().y
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

		if (Input::CheckHeld(BTN_SHIFT))
		{
			pCursorBox->m_sprite.visible = true;

			game::float2 loc{
				pCurrentCamera->ScreenLocToWorldLoc(
				Input::GetMouseLoc().x,
				Input::GetMouseLoc().y
				)
			};
			game::float2 locRounded{ roundf(loc.x), roundf(loc.y) };
			pCursorBox->m_location = locRounded;
			pCursorBox->Update(deltaTime);

			// Object spawn controls.
			if (Input::CheckPressed(BTN_LMB))
			{
				game::float2 loc{
					pCurrentCamera->ScreenLocToWorldLoc(
							Input::GetMouseLoc().x,
							Input::GetMouseLoc().y
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
		if (Input::CheckPressed(BTN_2)) current_prefab = PREFAB_Mushroom;
		if (Input::CheckPressed(BTN_3)) current_prefab = PREFAB_Floor_ConcreteA;
		if (Input::CheckPressed(BTN_4)) current_prefab = PREFAB_NPC;

		Collision();



		// Cursor
		game::float2 loc_px{ Input::GetMouseLoc().x, Input::GetMouseLoc().y };
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

Camera* Scene_World::GetCamera()
{
	return pCurrentCamera ? pCurrentCamera : nullptr;
}



Scene_Pause::Scene_Pause(bool visible)
	:
	Scene(visible),
	pResume(nullptr),
	pMainMenu(nullptr)
{
}
Scene_Pause::Scene_Pause(const Scene_Pause& scene)
	:
	Scene(scene),
	pResume(scene.pResume),
	pMainMenu(scene.pMainMenu)
{
}

void Scene_Pause::Initialise()
{
	pResume = nullptr;
	QueueToSpawn(PREFAB_Resume, { 0, 1 });
	pResume = dynamic_cast<SO_Button*>(vpSpawnQueue.back());

	pMainMenu = nullptr;
	QueueToSpawn(PREFAB_MainMenu, { 0, -1 });
	pMainMenu = dynamic_cast<SO_Button*>(vpSpawnQueue.back());

	SpawnObjects();
}

SceneMessage Scene_Pause::Update(float deltaTime)
{
	if (!canUpdate && isVisible)
	{
		pCursor->SetLocation_percentage({ 100.f,100.f });
		for (auto& pGameObject : vpGameObjects)
			pGameObject->Update_SpriteOnly();
	}
	else if (canUpdate)
	{
		SceneMessage msg{ SMID_Null, 0 };

		DestroyObjects();
		SpawnObjects();

		if (Input::CheckPressed(BTN_ESC))
		{
			msg.id = SMID_Pop;
		}
		if (pResume->IsPressed())
		{
			msg.id = SMID_Pop;
		}

		// Cursor
		game::float2 loc_px{ Input::GetMouseLoc().x, Input::GetMouseLoc().y };
		pCursor->SetLocaion_px(loc_px);

		// Update
		for (auto& pGameObject : vpGameObjects)
			pGameObject->Update(deltaTime);

		return msg;
	}
}
