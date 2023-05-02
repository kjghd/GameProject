#pragma once

#include "GameObject.h"

class ScreenObject : public GameObject
{
public:
	static game::float2 screenRes;
	static float px_per_su;

	ScreenObject(ImageData* sprite_pImageData, int sprite_layer, game::float2 sprite_scale = { 1.f,1.f }, game::float2 sprite_offset = { 0,0 }, float sprite_frameTime = 0);
	ScreenObject(const ScreenObject& screenObject);
	ScreenObject(const Sprite& sprite);

	virtual void Update(float deltaTime) override;

	void SetLocaion_px(game::float2 pixels);
	void SetLocaion_su(game::float2 screenUnits);
	void SetLocation_percentage(game::float2 percentage);

	int GetBitmapIndex();
	int GetRenderLayer();
	game::rect GetSourceRect();
	game::rect GetScreenRect();

	bool InvertedX();
	bool InvertedY();

	static bool CompareRenderOrder_Under(ScreenObject* screenObjectA, ScreenObject* screenObjectB);

	virtual std::string Serialise() override;
};

class SO_Preview : public ScreenObject
{
	game::float2 previewSize;
	Sprite* pPreviewed;

public:
	SO_Preview(game::float2 size, ImageData* sprite_pImageData, int sprite_layer, game::float2 sprite_scale = { 1.f,1.f }, game::float2 sprite_offset = { 0,0 }, float sprite_frameTime = 0);
	SO_Preview(const SO_Preview& box);
	SO_Preview(const Sprite& sprite, game::float2 size);
	~SO_Preview();

	virtual void Update(float deltaTime) override;

	void SetPreviewed(Sprite* pSprite = nullptr);
};
