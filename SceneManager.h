#pragma once
#include "Scene.h"
#include "Scene_World.h"
#include "Scene_Pause.h"
#include "Input.h"
#include <vector>


class Camera;
class ScreenObject;

class SceneManager
{
	Scene_World* pMainScene;
	std::vector<Scene*> pSceneStack;
	std::vector<Scene*> deleteQueue;
	Scene* pFocused;
	Camera* pCamera;
	Camera defaultCamera;
	ScreenObject* pCursor;

	Scene_Pause pause;

	void Pause();
	void Play();

	void LoadScene(std::string filename);
	//void NewScene(size_t index);
	void NewScene(std::string filename);
	void SaveScene(std::string filename);
	void SaveScenePrefab(Scene* pScene, std::string filename);
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
