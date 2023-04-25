#pragma once

#include "GameObject.h"

class ScreenObject : public GameObject
{
	static game::Float2 screenRes;

public:
	static float px_per_su;

	ScreenObject(ImageData* sprite_pImageData, int sprite_layer, game::Float2 sprite_scale = { 1.f,1.f }, game::Float2 sprite_offset = { 0,0 }, float sprite_frameTime = 0);
	ScreenObject(const ScreenObject& screenObject);

	virtual void Update(float deltaTime) override;

	void SetLocaion_px(game::Float2 pixels);
	void SetLocaion_su(game::Float2 screenUnits);
	void SetLocation_percentage(game::Float2 percentage);

	int GetBitmapIndex();
	int GetRenderLayer();
	game::Rect GetSourceRect();
	game::Rect GetScreenRect();

	bool InvertedX();
	bool InvertedY();

	static bool CompareRenderOrder_Under(ScreenObject* screenObjectA, ScreenObject* screenObjectB);
};

