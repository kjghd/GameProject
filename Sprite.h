#pragma once

#include "DataTypes.h"
#include "ImageData.h"

#include <vector>

class WorldObject;


enum SpriteLayers
{
	SL_UI_Front,
	SL_UI_Mid,
	SL_UI_Back,

	SL_FX,
	SL_Object,
	SL_Floor,

	SL_DEFAULT,

	SL_COUNT
};

class Sprite
{
protected:
	WorldObject* pOwner;
	game::Float2 offset;
	game::Float2 scale;

	int layer;
	ImageData* pImageData;
	int currentFrame;
	int currentAnim;
	float frameTimeMax;
	float frameTimeCurrent;
	int direction;
	bool invertedX;
	bool invertedY;

public:
	static std::vector<Sprite*> vpSpritesToRender;


	Sprite(WorldObject* pOwner, ImageData* pImageData, float frameTime, int layer = SL_DEFAULT, game::Float2 scale = { 1,1 }, game::Float2 offset = { 0,0 });
	Sprite(WorldObject* pOwner, const Sprite& sprite);

	void Update(float deltaTime);

	void Pause();
	void PlayForwards();
	void PlayBackwards();

	game::Float2 GetLocation();
	game::Float2 GetSize();

	int GetRenderLayer();
	game::Rect GetSourceRect();
	int GetBitmapIndex();
	int GetCurrentAnimation();
	bool CheckInvertedX();
	bool CheckInvertedY();

	void SetAnimation(int index);
	void FlipX();
	void FlipY();

	static bool CompareLayer(Sprite* pA, Sprite* pB);
	static bool CompareAbove(Sprite* pA, Sprite* pB);
	static bool CompareRowAndLeftOf(Sprite* gameObjectA, Sprite* gameObjectB);

};
