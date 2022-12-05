#pragma once
#include "Graphics.h"
#include "Scene.h"

class Renderer
{
	Graphics* m_pGraphics;
	Scene* m_pScene;
	Camera* m_pCamera;

public:

	Renderer();

	void Init(Graphics* pGraphics, Scene* pScene);

	void Render();
};
