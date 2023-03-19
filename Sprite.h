#pragma once

#include "DataTypes.h"
#include "ImageData.h"

enum SpriteLayers
{
	SL_UI,
	SL_FX,
	SL_Object,
	SL_Floor,
	SL_DEFAULT,

	SL_COUNT
};

class Sprite
{
protected:
	game::Float2& origin;
	game::Float2 offset;
	game::Float2 scale;

	int layer;
	ImageData* pImageData;
	int currentFrame;
	int currentAnim;
	float frameTimeMax;
	float frameTimeCurrent;

public:
	Sprite(game::Float2* pOrigin, ImageData* pImageData, float frameTime, int layer = SL_DEFAULT, game::Float2 scale = { 1,1 }, game::Float2 offset = { 0,0 });
	Sprite(game::Float2* pOrigin, ImageData* pImageData, int layer = SL_DEFAULT, game::Float2 scale = { 1,1 }, game::Float2 offset = { 0,0 });
	Sprite(game::Float2* pOrigin, const Sprite& sprite);

	void Update(float deltaTime);

	game::Float2 GetLocation();
	game::Float2 GetSize();

	int GetRenderLayer();
	game::Rect GetSourceRect();
	int GetBitmapIndex();
	int GetCurrentAnimation();
	void SetAnimation(int index);

	Sprite& operator =(const Sprite& sprite)
	{
		origin = sprite.origin;
		layer = sprite.layer;
		pImageData = sprite.pImageData;
		scale = sprite.scale;
		offset = sprite.offset;

		return *this;
	}
};
