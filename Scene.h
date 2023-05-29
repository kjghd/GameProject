#pragma once
#include "FileWritable.h"
#include "DataTypes.h"
#include "Camera.h"
#include <vector>
#include <string>


class GameObject;
class WorldObject;
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
	//int index;
	std::string fileName;
};

class Scene : public FileWritable
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

	virtual void WriteData(std::ostream& os) override;

public:
	Scene(bool visible);
	Scene(const Scene& scene);
	Scene(std::istream& is);
	~Scene();

	virtual void Initialise();

	virtual SceneMessage Update(float deltaTime);

	void Activate();
	void Deactivate(bool visible = false);
	bool CheckActive();

	void SetCursor(ScreenObject* pCursor);

	void QueueToSpawn(int prefab, game::float2 location = { 0,0 });
	void QueueToSpawn(GameObject* pObject);
};

class Scene_World : public Scene
{
protected:
	Camera* pCurrentCamera;
	int current_prefab;
	Player* pPlayer;
	WorldObject* pCursorBox;
	ScreenObject* pPreview;

	virtual void SpawnObjects() override;
	virtual void WriteData(std::ostream& os) override;

public:
	Scene_World(bool show);
	Scene_World(const Scene_World& scene);
	Scene_World(std::istream& is);

	Camera* GetCamera();

	virtual void Initialise() override;

	virtual SceneMessage Update(float deltaTime) override;
};

class Scene_Pause : public Scene
{
protected:
	SO_Button* pResume;
	SO_Button* pMainMenu;
	SO_Button* pSave;
	SO_Button* pLoad;
	SO_Button* pNew;

	virtual void WriteData(std::ostream& os) override;

public:

	Scene_Pause(bool visible);
	Scene_Pause(const Scene_Pause& scene);
	Scene_Pause(std::istream& is);
	virtual void Initialise() override;
	virtual SceneMessage Update(float deltaTime) override;
};
