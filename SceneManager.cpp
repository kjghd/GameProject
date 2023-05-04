#include "SceneManager.h"

#include "Prefabs.h"
#include "PrefabTags.h"


std::vector<Scene*> SceneManager::pScenePrefabs;
std::vector<Scene*> SceneManager::pSceneStore;


SceneManager::SceneManager()
	:
	pFocused(nullptr),
	defaultCamera(nullptr)
{
}
SceneManager::~SceneManager()
{
	for (auto& pScene : pSceneStack)
	{
		delete pScene;
		pScene = nullptr;
	}

	for (auto& pScene : pScenePrefabs)
	{
		delete pScene;
		pScene = nullptr;
	}

	for (auto& pScene : pSceneStore)
	{
		delete pScene;
		pScene = nullptr;
	}
}

void SceneManager::Initialise()
{
	// Level 1
	Scene_World* pLevel1{ new Scene_World(true) };
	pLevel1->QueueToSpawn(PREFAB_Mushroom, { 0,5 });
	pLevel1->QueueToSpawn(PREFAB_Mushroom, { 1,5 });
	pLevel1->QueueToSpawn(PREFAB_Mushroom, { 2,5 });
	pLevel1->QueueToSpawn(PREFAB_Mushroom, { 3,5 });
	pLevel1->QueueToSpawn(PREFAB_Mushroom, { 4,5 });
	pScenePrefabs.push_back(pLevel1);

	// Pause
	Scene* pPause{ new Scene(false) };
	pPause->QueueToSpawn(PREFAB_Background, { 0,0 });
	pPause->QueueToSpawn(PREFAB_Resume, { 0,1 });
	pPause->QueueToSpawn(PREFAB_MainMenu, { 0,-1 });
	pScenePrefabs.push_back(pPause);

	NewScene(0);
	pFocused = pSceneStack.back();
}

void SceneManager::Update(float deltaTime)
{
	while (!deleteQueue.empty())
	{
		delete deleteQueue.back();
		deleteQueue.back() = nullptr;
		deleteQueue.pop_back();
	}

	for (const auto& pScene : pSceneStack)
	{
		SceneMessage sm{ pScene->Update(deltaTime) };

		if (pScene == pFocused)
		{
			switch (sm.id)
			{
			case SMID_Null: break;
			case SMID_Pop: PopScene(); break;
			case SMID_Store: SaveScene(); break;
			case SMID_Load: LoadScene(sm.indexStore); break;
			case SMID_New: NewScene(sm.indexPrefabs);  break;
			}
		}
	}

	if (pMainScene)
		pCamera = pMainScene->GetCamera();
	else
		pCamera = &defaultCamera;

}

void SceneManager::LoadScene(size_t storeIndex)
{
	if (dynamic_cast<Scene_World*>(pSceneStore.at(storeIndex)))
	{
		pSceneStack.push_back(new Scene_World(*dynamic_cast<Scene_World*>(pSceneStore.at(storeIndex))));
		pMainScene = dynamic_cast<Scene_World*>(pSceneStack.back());
	}
	else
		pSceneStack.push_back(new Scene(*pSceneStore.at(storeIndex)));

	pSceneStack.back()->Initialise();
	pSceneStack.back()->Activate();
	pFocused = pSceneStack.back();

}
void SceneManager::NewScene(size_t prefabIndex)
{
	if (dynamic_cast<Scene_World*>(pScenePrefabs.at(prefabIndex)))
	{
		pSceneStack.push_back(new Scene_World(*dynamic_cast<Scene_World*>(pScenePrefabs.at(prefabIndex))));
		pMainScene = dynamic_cast<Scene_World*>(pSceneStack.back());
	}
	else
		pSceneStack.push_back(new Scene(*pScenePrefabs.at(prefabIndex)));

	pSceneStack.back()->Initialise();
	pSceneStack.back()->Activate();
	pFocused = pSceneStack.back();
}
void SceneManager::SaveScene()
{
	pSceneStore.push_back(pMainScene);
}
void SceneManager::PopScene()
{
	if (!pSceneStack.empty())
	{
		deleteQueue.push_back(pSceneStack.back());
		pSceneStack.pop_back();

		if (!pSceneStack.empty())
			pFocused = pSceneStack.back();
		else
			pFocused = nullptr;
	}
}

void SceneManager::SetActive(int index)
{
	//for (auto& pScene : pMenuStack)
	//	pScene->Deactivate();

	pSceneStack.at(index)->Activate();
	pFocused = pSceneStack.at(index);
}
//Scene* SceneManager::GetActive()
//{
//	return pFocused;
//}
Camera* SceneManager::GetCamera()
{
	return pCamera;
}