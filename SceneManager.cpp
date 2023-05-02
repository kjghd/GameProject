#include "SceneManager.h"

#include "Prefabs.h"
#include "PrefabTags.h"


std::vector<Scene*> SceneManager::prefabs;
std::vector<Scene*> SceneManager::store;

void SceneManager::Initialise()
{
	// Level 1
	Scene_World* pLevel1{ new Scene_World(true) };
	pLevel1->QueueToSpawn(PREFAB_Mushroom, { 0,5 });
	pLevel1->QueueToSpawn(PREFAB_Mushroom, { 1,5 });
	pLevel1->QueueToSpawn(PREFAB_Mushroom, { 2,5 });
	pLevel1->QueueToSpawn(PREFAB_Mushroom, { 3,5 });
	pLevel1->QueueToSpawn(PREFAB_Mushroom, { 4,5 });
	prefabs.push_back(pLevel1);

	// Pause
	Scene* pPause{ new Scene(false) };
	pPause->QueueToSpawn(PREFAB_Background, { 0,0 });
	pPause->QueueToSpawn(PREFAB_Resume, { 0,1 });
	pPause->QueueToSpawn(PREFAB_MainMenu, { 0,-1 });
	prefabs.push_back(pPause);
}

SceneManager::SceneManager()
	:
	pActive(nullptr)
{
}

SceneManager::~SceneManager()
{
	for (auto& pScene : stack)
	{
		delete pScene;
		pScene = nullptr;
	}

	for (auto& pScene : prefabs)
	{
		delete pScene;
		pScene = nullptr;
	}

	for (auto& pScene : store)
	{
		delete pScene;
		pScene = nullptr;
	}
}

void SceneManager::Update(float deltaTime)
{
	while (!deleteQueue.empty())
	{
		delete deleteQueue.back();
		deleteQueue.back() = nullptr;
		deleteQueue.pop_back();
	}

	if (stack.empty())
	{
		stack.push_back(new Scene_World(*dynamic_cast<Scene_World*>(prefabs.at(0))));
		
		stack.back()->Initialise();
		SetActive(0);
	}

	for (const auto& pScene : stack)
	{
		SceneMessage sm{ pScene->Update(deltaTime) };
		if (pScene == pActive)
		{
			switch (sm.id)
			{
				case SMID_Null: break;
				case SMID_Pop:
				{
					stack.back()->Dectivate();
					deleteQueue.push_back(stack.back());
					stack.pop_back();
					stack.back()->Activate();
					pActive = stack.back();
					break;
				}
				case SMID_Store:
				{
					stack.back()->Dectivate();
					store.push_back(pActive);
					stack.pop_back();
					stack.back()->Activate();
					//pActive = stack.back();
					break;
				}
				case SMID_Load:
				{
					stack.back()->Dectivate();
					stack.push_back(store.at(sm.indexStore));
					store.erase(store.begin() + sm.indexStore);
					stack.back()->Activate();
					//pActive = stack.back();
					break;
				}
				case SMID_New:
				{
					stack.back()->Dectivate();
					if (dynamic_cast<Scene_World*>(prefabs.at(sm.indexPrefabs)))
					{
						stack.push_back(new Scene_World(*dynamic_cast<Scene_World*>(prefabs.at(sm.indexPrefabs))));
					}
					else
					{
						stack.push_back(new Scene(*prefabs.at(sm.indexPrefabs)));
					}
					stack.back()->Initialise();
					stack.back()->Activate();
					pActive = stack.back();
					break;
				}
			}
		}
	}
}

void SceneManager::SetActive(int index)
{
	for (auto& pScene : stack)
		pScene->Dectivate();

	stack.at(index)->Activate();
	pActive = stack.at(index);
}

Scene* SceneManager::GetActive()
{
	return pActive;
}
