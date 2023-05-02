#pragma once

#include "Scene.h"
#include "Input.h"

#include <vector>

class Camera;
class ScreenObject;

class SceneManager
{
	static std::vector<Scene*> prefabs;	// Will be replaced with files.
	static std::vector<Scene*> store;	// Will be replaced with files.
	std::vector<Scene*> stack;
	std::vector<Scene*> deleteQueue;
	Scene* pActive;
	Camera* pCamera;
	ScreenObject* pCursor;

public:
	SceneManager();
	~SceneManager();
	
	void Initialise();

	void Update(float deltaTime);
	void SetActive(int index);
	Scene* GetActive();
};
