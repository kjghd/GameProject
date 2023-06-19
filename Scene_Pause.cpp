#include "Scene_Pause.h"
#include "Input.h"
#include "Prefabs.h"

#include "SO_Button.h"


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

	QueueToSpawn(PREFAB_Background);

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
			msg.id = SMID_Play;
		}
		if (pResume->IsPressed())
		{
			msg.id = SMID_Play;
		}
		if (pSave->IsPressed())
		{
			msg.id = SMID_Save;
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
