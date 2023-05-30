#pragma once
#include "Scene.h"

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
