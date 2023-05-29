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
	canUpdate(false),
	pCursor(nullptr)
{
	m_tag = "SCNE";
}
Scene::Scene(const Scene& scene)
	:
	isVisible(scene.isVisible),
	canUpdate(scene.canUpdate),
	vpSpawnQueue(),
	pCursor(scene.pCursor)
{
	m_tag = "SCNE";

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
Scene::Scene(std::istream& is)
	:
	isVisible(FileWritable::GetNextValue(is).c_str() == "1" ? true : false),
	canUpdate(FileWritable::GetNextValue(is).c_str() == "1" ? true : false),
	pCursor(nullptr)
{
	int objectCount{ std::stoi(FileWritable::GetNextValue(is).c_str()) };
	while (objectCount > 0)
	{
		std::string tag = FileWritable::GetNextValue(is);
		if (tag == "GOBJ") vpGameObjects.push_back(new GameObject(is));
		else if (tag == "WOBJ") vpGameObjects.push_back(new WorldObject(is));
		else if (tag == "WOBX") vpGameObjects.push_back(new Box(is));
		else if (tag == "WOBL") vpGameObjects.push_back(new Ball(is));
		else if (tag == "WOCH") vpGameObjects.push_back(new Character(is));
		else if (tag == "WONP") vpGameObjects.push_back(new NPC(is));
		else if (tag == "WOPC") vpGameObjects.push_back(new Player(is));
		else if (tag == "SOBJ") vpGameObjects.push_back(new ScreenObject(is));
		else if (tag == "SOBT") vpGameObjects.push_back(new SO_Button(is));
		--objectCount;
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
	SceneMessage msg{ SMID_Null, 0 };
	if (!canUpdate && isVisible)
	{
		pCursor->SetLocation_percentage({ 100.f,100.f });
		for (auto& pGameObject : vpGameObjects)
			pGameObject->Update_SpriteOnly();
	}
	else if (canUpdate)
	{
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
	}
	return msg;
}   

void Scene::QueueToSpawn(int prefab, game::float2 location)
{
	vpSpawnQueue.push_back(PrefabList::Get(prefab)->Clone());
	vpSpawnQueue.back()->m_location = location;
}

void Scene::QueueToSpawn(GameObject* pObject)
{
	if (pObject)
	{
		vpSpawnQueue.push_back(pObject->Clone());
		vpSpawnQueue.back()->m_location = pObject->m_location;
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

void Scene::SetCursor(ScreenObject* pC)
{
	pCursor = pC;
}

void Scene::WriteData(std::ostream& os)
{
	DestroyObjects();
	SpawnObjects();

	os << isVisible << ',';
	os << canUpdate << ',';
	os << vpGameObjects.size() << '\n';

	for (const auto& pGameObject : vpGameObjects)
	{
		pGameObject->Write(os);
		os << '\n';
	}
}


// World

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
	pCurrentCamera(nullptr),
	current_prefab(PREFAB_Floor_ConcreteA),
	pPlayer(nullptr),
	pCursorBox(nullptr),
	pPreview(nullptr)
{
	m_tag = "SCWL";
}
Scene_World::Scene_World(const Scene_World& scene)
	:
	Scene(scene),
	pCurrentCamera(nullptr),
	current_prefab(PREFAB_Floor_ConcreteA),
	pPlayer(nullptr),
	pCursorBox(nullptr),
	pPreview(nullptr)
{
	m_tag = "SCWL";
}
Scene_World::Scene_World(std::istream& is)
	:
	Scene(is),
	pCurrentCamera(nullptr),
	current_prefab(std::stoi(FileWritable::GetNextValue(is))),
	pPlayer(nullptr),
	pCursorBox(nullptr),
	pPreview(nullptr)
{
	m_tag = "SCWL";

	int n_pPlayer = std::stoi(FileWritable::GetNextValue(is));
	if (n_pPlayer >= 0)
	{
		pPlayer = dynamic_cast<Player*>(vpGameObjects.at(n_pPlayer));
	}

	int n_pCursorBox = std::stoi(FileWritable::GetNextValue(is));
	if (n_pCursorBox >= 0)
	{
		pCursorBox = dynamic_cast<WorldObject*>(vpGameObjects.at(n_pCursorBox));
	}

	int n_pPreview = std::stoi(FileWritable::GetNextValue(is));
	if (n_pPreview >= 0)
	{
		pPreview = dynamic_cast<ScreenObject*>(vpGameObjects.at(n_pPreview));
	}
}


SceneMessage Scene_World::Update(float deltaTime)
{
	SceneMessage msg{ SMID_Null };
	if (!canUpdate && isVisible)
	{
		for (auto& pGameObject : vpGameObjects)
			pGameObject->Update_SpriteOnly();
	}
	else if (canUpdate)
	{
		DestroyObjects();
		SpawnObjects();

		if (pPlayer)
		{
			if (!pCurrentCamera) pCurrentCamera = &pPlayer->m_camera;
		}

		if (Input::CheckPressed(BTN_ESC))
		{
			msg.id = SMID_New;
			//msg.index = SCENE_Pause;
			msg.fileName = "pause.scene";
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
			if (Input::CheckHeld(BTN_LMB))
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
		if (Input::CheckPressed(BTN_1)) current_prefab = PREFAB_Mushroom;
		if (Input::CheckPressed(BTN_2)) current_prefab = PREFAB_Floor_ConcreteA;
		if (Input::CheckPressed(BTN_3)) current_prefab = PREFAB_NPC;

		Collision();

		// Cursor
		game::float2 loc_px{ Input::GetMouseLoc().x, Input::GetMouseLoc().y };
		pCursor->SetLocaion_px(loc_px);

		// Update
		for (auto& pGameObject : vpGameObjects)
			pGameObject->Update(deltaTime);
	}
	return msg;
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

void Scene_World::WriteData(std::ostream& os)
{
	Scene::WriteData(os);
	//os << ',';

	os << current_prefab << ',';

	if (pPlayer)
	{
		int i{ 0 };
		for (const auto& pObject : vpGameObjects)
		{
			if (pObject == pPlayer)
			{
				os << i;
				break;
			}
			++i;
		}
	}
	else
	{
		os << -1;
	}
	os << ',';

	if (pCursorBox)
	{
		int i{ 0 };
		for (const auto& pObject : vpGameObjects)
		{
			if (pObject == pCursorBox)
			{
				os << i;
				break;
			}
			++i;
		}
	}
	else
	{
		os << -1;
	}
	os << ',';

	if (pPreview)
	{
		int i{ 0 };
		for (const auto& pObject : vpGameObjects)
		{
			if (pObject == pPreview)
			{
				os << i;
				break;
			}
			++i;
		}
	}
	else
	{
		os << -1;
	}
}


// Pause

Scene_Pause::Scene_Pause(bool visible)
	:
	Scene(visible),
	pResume(nullptr),
	pMainMenu(nullptr),
	pSave(nullptr),
	pLoad(nullptr),
	pNew(nullptr)
{
	m_tag = "SCPS";
}
Scene_Pause::Scene_Pause(const Scene_Pause& scene)
	:
	Scene(scene),
	pResume(scene.pResume),
	pMainMenu(scene.pMainMenu),
	pSave(scene.pSave),
	pLoad(scene.pLoad),
	pNew(scene.pNew)
{
	m_tag = "SCPS";
}
Scene_Pause::Scene_Pause(std::istream& is)
	:
	Scene(is),
	pResume(nullptr),
	pMainMenu(nullptr),
	pSave(nullptr),
	pLoad(nullptr),
	pNew(nullptr)
{
	m_tag = "SCWL";

	int n_pResume = std::stoi(FileWritable::GetNextValue(is));
	if (n_pResume >= 0)
	{
		pResume = dynamic_cast<SO_Button*>(vpGameObjects.at(n_pResume));
	}

	int n_pMainMenu = std::stoi(FileWritable::GetNextValue(is));
	if (n_pMainMenu >= 0)
	{
		pMainMenu = dynamic_cast<SO_Button*>(vpGameObjects.at(n_pMainMenu));
	}

	int n_pSave = std::stoi(FileWritable::GetNextValue(is));
	if (n_pSave >= 0)
	{
		pSave = dynamic_cast<SO_Button*>(vpGameObjects.at(n_pSave));
	}

	int n_pLoad = std::stoi(FileWritable::GetNextValue(is));
	if (n_pLoad >= 0)
	{
		pLoad = dynamic_cast<SO_Button*>(vpGameObjects.at(n_pLoad));
	}

	int n_pNew = std::stoi(FileWritable::GetNextValue(is));
	if (n_pNew >= 0)
	{
		pNew = dynamic_cast<SO_Button*>(vpGameObjects.at(n_pNew));
	}
}

void Scene_Pause::Initialise()
{
	if (!pResume)
	{
		QueueToSpawn(PREFAB_Resume, { 0, 3 });
		pResume = dynamic_cast<SO_Button*>(vpSpawnQueue.back());
	}

	if (!pSave)
	{
		QueueToSpawn(PREFAB_Save, { 0, 1.5 });
		pSave = dynamic_cast<SO_Button*>(vpSpawnQueue.back());
	}

	if (!pLoad)
	{
		QueueToSpawn(PREFAB_Load, { 0, 0 });
		pLoad = dynamic_cast<SO_Button*>(vpSpawnQueue.back());
	}

	if (!pNew)
	{
		QueueToSpawn(PREFAB_New, { 0, -1.5 });
		pNew = dynamic_cast<SO_Button*>(vpSpawnQueue.back());
	}

	if (!pMainMenu)
	{
		QueueToSpawn(PREFAB_MainMenu, { 0, -3 });
		pMainMenu = dynamic_cast<SO_Button*>(vpSpawnQueue.back());
	}


	SpawnObjects();
}

SceneMessage Scene_Pause::Update(float deltaTime)
{
	SceneMessage msg{ SMID_Null };
	if (!canUpdate && isVisible)
	{
		pCursor->SetLocation_percentage({ 100.f,100.f });
		for (auto& pGameObject : vpGameObjects)
			pGameObject->Update_SpriteOnly();
	}
	else if (canUpdate)
	{
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
		if (pSave->IsPressed())
		{
			msg.id = SMID_Store;
			msg.fileName = "save.scene";
		}
		if (pLoad->IsPressed())
		{
			msg.id = SMID_Load;
			msg.fileName = "save.scene";
		}
		if (pNew->IsPressed())
		{
			msg.id = SMID_New;
			msg.fileName = "empty.scene";
		}


		// Cursor
		game::float2 loc_px{ Input::GetMouseLoc().x, Input::GetMouseLoc().y };
		pCursor->SetLocaion_px(loc_px);

		// Update
		for (auto& pGameObject : vpGameObjects)
			pGameObject->Update(deltaTime);
	}
	return msg;
}

void Scene_Pause::WriteData(std::ostream& os)
{
	Scene::WriteData(os);

	if (pResume)
	{
		int i{ 0 };
		for (const auto& pObject : vpGameObjects)
		{
			if (pObject == pResume)
			{
				os << i;
				break;
			}
			++i;
		}
	}
	else
	{
		os << -1;
	}
	os << ',';

	if (pMainMenu)
	{
		int i{ 0 };
		for (const auto& pObject : vpGameObjects)
		{
			if (pObject == pMainMenu)
			{
				os << i;
				break;
			}
			++i;
		}
	}
	else
	{
		os << -1;
	}
	os << ',';

	if (pSave)
	{
		int i{ 0 };
		for (const auto& pObject : vpGameObjects)
		{
			if (pObject == pSave)
			{
				os << i;
				break;
			}
			++i;
		}
	}
	else
	{
		os << -1;
	}
	os << ',';

	if (pLoad)
	{
		int i{ 0 };
		for (const auto& pObject : vpGameObjects)
		{
			if (pObject == pLoad)
			{
				os << i;
				break;
			}
			++i;
		}
	}
	else
	{
		os << -1;
	}
	os << ',';

	if (pNew)
	{
		int i{ 0 };
		for (const auto& pObject : vpGameObjects)
		{
			if (pObject == pNew)
			{
				os << i;
				break;
			}
			++i;
		}
	}
	else
	{
		os << -1;
	}
}
