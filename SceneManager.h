#pragma once
#include "Scene.h"
#include "Input.h"
#include <vector>


class Camera;
class ScreenObject;

class SceneManager
{
	Scene_World* pMainScene;
	static std::vector<Scene*> pScenePrefabs;	// Will be replaced with files.
	static std::vector<Scene*> pSceneStore;	// Will be replaced with files.
	std::vector<Scene*> pSceneStack;
	std::vector<Scene*> deleteQueue;
	Scene* pFocused;
	Camera* pCamera;
	Camera defaultCamera;
	ScreenObject* pCursor;

	void LoadScene(size_t storeIndex);
	void NewScene(size_t index);
	void SaveScene();
	void PopScene();

	void FocusWorld();
	void FocusMenu();

public:
	SceneManager();
	~SceneManager();
	
	void Initialise();

	void Update(float deltaTime);

	void SetActive(int index);
	void SetActive(Scene* pScene);

	Camera* GetCamera();
};
