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
