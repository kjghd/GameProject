#pragma once
#include "DataTypes.h"
#include "Camera.h"
#include <vector>
#include <string>


class GameObject;
class Player;
class ScreenObject;
class SO_Button;

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
	bool isVisible;
	bool canUpdate;

	std::vector<GameObject*> vpSpawnQueue;
	std::vector<size_t> vDestroyQueue;
	std::vector<GameObject*> vpGameObjects;
	ScreenObject* pCursor;

	void Collision();

	void QueueToDestroy(size_t tileIndex);

	virtual void SpawnObjects();
	void DestroyObjects();

public:
	Scene(bool visible);
	Scene(const Scene& scene);
	~Scene();

	virtual void Initialise();

	virtual SceneMessage Update(float deltaTime);

	void Activate();
	void Deactivate(bool visible = false);
	bool CheckActive();

	void SetCursor(ScreenObject* pCursor);

	void QueueToSpawn(int prefab, game::float2 location = { 0,0 });
	void QueueToSpawn(GameObject* pObject);

	std::string Serialise();
};

class Scene_World : public Scene
{
protected:
	Camera* pCurrentCamera;
	int current_prefab;
	Player* pPlayer;
	GameObject* pCursorBox;
	ScreenObject* pPreview;

	virtual void SpawnObjects() override;

public:
	Scene_World(bool show);
	Scene_World(const Scene_World& scene);

	Camera* GetCamera();

	virtual void Initialise() override;

	virtual SceneMessage Update(float deltaTime) override;
};

class Scene_Pause : public Scene
{
	SO_Button* pResume;
	SO_Button* pMainMenu;
public:
	Scene_Pause(bool visible);
	Scene_Pause(const Scene_Pause& scene);
	virtual void Initialise() override;
	virtual SceneMessage Update(float deltaTime) override;
};
