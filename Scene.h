#pragma once

#include "DataTypes.h"
#include "Camera.h"
#include <vector>
#include <string>

class GameObject;
class Player;
class ScreenObject;


enum ScenePrefabs
{
	SCENE_Level1,
	SCENE_Pause
};

enum SceneMessageID
{
	SMID_Null,	// Do nothing.
	SMID_Pop,	// Remove without storing.
	SMID_Store,	// Remove and store.
	SMID_Load,	// Push from store.
	SMID_New	// Push from prefabs.
};

struct SceneMessage
{
	int id{ SMID_Null };
	int indexStore{ 0 };  // Use until file loading works.
	int indexPrefabs{ 0 }; // Use until file loading works.
	//std::string fileName;
};

class Scene
{
protected:
	bool show;
	bool active;

	Camera* pCurrentCamera;
	Camera defaultCamera;
	ScreenObject* pCursor;

	std::vector<GameObject*> vpSpawnQueue;
	std::vector<size_t> vDestroyQueue;
	std::vector<GameObject*> vpGameObjects;

	void Collision();

	void QueueToDestroy(size_t tileIndex);

	virtual void SpawnObjects();
	void DestroyObjects();

public:
	Scene(bool show);
	Scene(const Scene& scene);
	~Scene();

	virtual void Initialise();

	void Activate();
	void Dectivate();
	bool CheckActive();

	Camera* GetCamera();

	void QueueToSpawn(int prefab, game::float2 location = { 0,0 });
	void QueueToSpawn(GameObject* pObject);

	virtual SceneMessage Update(float deltaTime);

	std::string Serialise();
};

class Scene_World : public Scene
{
	int current_prefab;
	Player* pPlayer;
	GameObject* pCursorBox;

	virtual void SpawnObjects() override;

public:
	Scene_World(bool show);
	Scene_World(const Scene_World& scene);
	virtual void Initialise() override;
	virtual SceneMessage Update(float deltaTime) override;


};

class Scene_Menu : public Scene
{

};