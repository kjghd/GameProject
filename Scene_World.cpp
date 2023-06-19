#include "Scene_World.h"
#include "Input.h"
#include "Prefabs.h"

#include "Player.h"
#include "ScreenObject.h"
#include "Character.h"


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
	current_prefab(PREFAB_Field),
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
	current_prefab(scene.current_prefab),
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
			msg.id = SMID_Pause;
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
			std::srand(static_cast<unsigned int>(deltaTime));

			pCursorBox->m_sprite.visible = true;

			// Round mouse loc to whole unit.
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
			GameObject* pSpawn{ PrefabList::Get(current_prefab) };
			if (Input::CheckPressed(BTN_LMB))
			{
				bool canSpawn{ true };
				for (auto& pGameObject : vpGameObjects)
				{
					if (pGameObject->m_location == locRounded &&
						pSpawn->m_sprite.GetRenderLayer() == pGameObject->m_sprite.GetRenderLayer() &&
						pGameObject != pCursorBox)
					{
						canSpawn = false;
					}
				}

				if (canSpawn)
				{
					QueueToSpawn(current_prefab, locRounded);
					if (current_prefab == PREFAB_Field)
					{
						switch (std::rand() % 4)
						{
						case 0: vpSpawnQueue.back()->m_sprite.SetAnimation("A"); break;
						case 1: vpSpawnQueue.back()->m_sprite.SetAnimation("B"); break;
						case 2: vpSpawnQueue.back()->m_sprite.SetAnimation("C"); break;
						case 3: vpSpawnQueue.back()->m_sprite.SetAnimation("D"); break;
						}
					}
				}
			}
		}
		else
			pCursorBox->m_sprite.visible = false;
		if (Input::CheckPressed(BTN_1))
		{
			current_prefab = PREFAB_Mushroom;
		}
		if (Input::CheckPressed(BTN_2)) current_prefab = PREFAB_Field;
		if (Input::CheckPressed(BTN_3)) current_prefab = PREFAB_NPC;

		if (Input::CheckPressed(BTN_F))
		{
			QueueToSpawn(PREFAB_TextBox);
		}

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
