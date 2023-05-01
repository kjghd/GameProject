#pragma once
#include "Graphics.h"
#include "SceneManager.h"

class Renderer
{
	Graphics* m_pGraphics;
	SceneController* m_pSceneController;
	bool m_debug;

public:

	Renderer();

	void Init(Graphics* pGraphics, SceneController* pSceneController);

	void Render();
};
