#pragma once
#include "DataTypes.h"
#include "Sprite.h"
#include "Input.h"

class ScreenObject
{
	static game::Float2 screenRes;
	static float px_per_su;

	Input* pInput;

	game::Float2 location;
	Sprite sprite;

public:
	ScreenObject(ImageData* sprite_pImageData, int sprite_layer, game::Float2 sprite_scale = { 1.f,1.f }, game::Float2 sprite_offset = { 0,0 }, float sprite_frameTime = 0);
	ScreenObject(const ScreenObject& screenObject);

	void Update(float deltaTime);

	bool Hovered();
	bool Pressed();

	void SetInput(Input* pInput);
	void SetLocaion_px(game::Float2 pixels);
	void SetLocaion_su(game::Float2 screenUnits);
	void SetLocation_percentage(game::Float2 percentage);

	int GetBitmapIndex();
	int GetRenderLayer();
	game::Rect GetSourceRect();
	game::Rect GetScreenRect();
};

