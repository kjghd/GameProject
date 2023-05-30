#pragma once
#include "Scene.h"


class ScreenObject;
class WorldObject;
class Player;

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
