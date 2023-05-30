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
}

void SceneManager::Initialise()
{
	pCursor = new ScreenObject(*dynamic_cast<ScreenObject*>(PrefabList::Get(PREFAB_Cursor)));

	// Pause
	//Scene_Pause* pPause{ new Scene_Pause(false) };
	//pPause->QueueToSpawn(PREFAB_Background, { 0,0 });
	//pPause->Initialise();
	//SaveScenePrefab(pPause, "pause.scene");

	// 
	NewScene("empty.scene");
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
			case SMID_Store: SaveScene(sm.fileName); break;
			case SMID_Load: LoadScene(sm.fileName);  break;
			case SMID_New: NewScene(sm.fileName);  break;
			}
		}
	}

	pCursor->Update(deltaTime);

	if (pMainScene)
		pCamera = pMainScene->GetCamera();
	else
		pCamera = &defaultCamera;
}

void SceneManager::LoadScene(std::string filename)
{
	std::ifstream fin("Data/Saves/" + filename);

	if (FileWritable::GetNextValue(fin) == "SCWL")
	{
		while (!pSceneStack.empty())
			PopScene();

		pSceneStack.push_back(new Scene_World(fin));
		pMainScene = dynamic_cast<Scene_World*>(pSceneStack.back());
	}

	fin.close();


	pSceneStack.back()->SetCursor(pCursor);
	SetActive(pSceneStack.back());

}
void SceneManager::NewScene(std::string filename)
{
	std::ifstream fin("Data/Scenes/" + filename);

	std::string tag{ FileWritable::GetNextValue(fin) };

	if (tag == "SCWL")
	{
		while (!pSceneStack.empty())
			PopScene();

		pSceneStack.push_back(new Scene_World(fin));
		pMainScene = dynamic_cast<Scene_World*>(pSceneStack.back());
	}
	else if (tag == "SCPS")
	{
		pSceneStack.push_back(new Scene_Pause(fin));
	}

	fin.close();


	pSceneStack.back()->Initialise();
	pSceneStack.back()->SetCursor(pCursor);
	SetActive(pSceneStack.back());
}
void SceneManager::SaveScene(std::string filename)
{
	std::ofstream fout("Data/Saves/" + filename);
	pMainScene->Write(fout);
	fout.close();
}
void SceneManager::SaveScenePrefab(Scene* pScene, std::string filename)
{
	std::ofstream fout("Data/Scenes/" + filename);
	pScene->Write(fout);
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
