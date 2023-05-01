#pragma once
#include "Scene.h"
#include "Input.h"
#include <vector>

class SceneController
{
	Input* pInput;
	static std::vector<Scene*> prefabs;	// Will be replaced with files.
	static std::vector<Scene*> store;	// Will be replaced with files.
	std::vector<Scene*> stack;
	Scene* pActive;

	std::vector<Scene*> deleteQueue;

public:
	void Intialise(Input* pInput);
	
	SceneController();

	~SceneController();

	void Update(float deltaTime);
	void SetActive(int index);
	Scene* GetActive();
};
