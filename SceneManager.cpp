#include "SceneManager.h"

#include "Prefabs.h"
#include "PrefabTags.h"

#include "GameObject.h"
#include "ScreenObject.h"
#include "SO_Button.h"
#include "WorldObject.h"
#include "Box.h"
#include "Ball.h"
#include "Character.h"
#include "NPC.h"
#include "Player.h"


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
	pCursor = new ScreenObject(*dynamic_cast<ScreenObject*>(PrefabList::Get(PREFAB_Cursor)));

	// Level 1
	Scene_World* pLevel1{ new Scene_World(true) };
	pLevel1->QueueToSpawn(PREFAB_Mushroom, { 0,5 });
	pLevel1->QueueToSpawn(PREFAB_Mushroom, { 1,2 });
	pLevel1->QueueToSpawn(PREFAB_Mushroom, { 4,-6 });
	pLevel1->QueueToSpawn(PREFAB_Mushroom, { 3,5 });
	pLevel1->QueueToSpawn(PREFAB_Mushroom, { 4,5 });
	pScenePrefabs.push_back(pLevel1);

	// Pause
	Scene_Pause* pPause{ new Scene_Pause(false) };
	pPause->QueueToSpawn(PREFAB_Background, { 0,0 });
	pScenePrefabs.push_back(pPause);

	// 
	NewScene(0);
	pFocused = pSceneStack.back();
	pMainScene = dynamic_cast<Scene_World*>(pSceneStack.back());
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
			case SMID_Load: {
				while (!pSceneStack.empty())
					PopScene();

				LoadScene(sm.indexStore);
				break;
			}
			case SMID_New: NewScene(sm.indexPrefabs);  break;
			}
		}
	}

	pCursor->Update(deltaTime);

	if (pMainScene)
		pCamera = pMainScene->GetCamera();
	else
		pCamera = &defaultCamera;
}

void SceneManager::LoadScene(size_t storeIndex)
{
	std::ifstream fin("Data/Saves/TestScene.scene");
	
	if (FileWritable::GetNextValue(fin) == "SCWL")
	{
		pSceneStack.push_back(new Scene_World(fin));
		pMainScene = dynamic_cast<Scene_World*>(pSceneStack.back());
	}


	pSceneStack.back()->SetCursor(pCursor);
	SetActive(pSceneStack.back());

}
void SceneManager::NewScene(size_t prefabIndex)
{
	if (dynamic_cast<Scene_World*>(pScenePrefabs.at(prefabIndex)))
	{
		pSceneStack.push_back(new Scene_World(*dynamic_cast<Scene_World*>(pScenePrefabs.at(prefabIndex))));
		pMainScene = dynamic_cast<Scene_World*>(pSceneStack.back());
	}
	else if (dynamic_cast<Scene_Pause*>(pScenePrefabs.at(prefabIndex)))
	{
		pSceneStack.push_back(new Scene_Pause(*dynamic_cast<Scene_Pause*>(pScenePrefabs.at(prefabIndex))));
	}
	else
		pSceneStack.push_back(new Scene(*pScenePrefabs.at(prefabIndex)));

	pSceneStack.back()->Initialise();
	pSceneStack.back()->SetCursor(pCursor);
	SetActive(pSceneStack.back());
}
void SceneManager::SaveScene()
{
	std::ofstream fout("Data/Saves/TestScene.scene");
	pMainScene->Write(fout);
	fout.close();
}
void SceneManager::PopScene()
{
	if (!pSceneStack.empty())
	{
		deleteQueue.push_back(pSceneStack.back());
		pSceneStack.pop_back();

		if (!pSceneStack.empty())
			SetActive(pSceneStack.back());
		else
			pFocused = nullptr;
	}
}

void SceneManager::SetActive(int index)
{
	for (auto& pScene : pSceneStack)
		pScene->Deactivate();

	pSceneStack.at(index)->Activate();
	pFocused = pSceneStack.at(index);
}
void SceneManager::SetActive(Scene* pTargetScene)
{
	for (auto& pScene : pSceneStack)
		pScene->Deactivate();

	pTargetScene->Activate();
	pFocused = pTargetScene;
}
Camera* SceneManager::GetCamera()
{
	return pCamera;
}
