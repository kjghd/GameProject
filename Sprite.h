#pragma once

#include "DataTypes.h"
#include "ImageData.h"
#include "Camera.h"

#include <vector>

class GameObject;

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

struct RenderData
{
	RenderData(game::Float2 location, game::Float2 size, int bitmapIndex, int layer, game::Rect sourceRect, bool invertX, bool invertY);

private:

	GameObject* pOwner;

	game::Float2 location;
	game::Float2 size;

	int bitmapIndex;
	int layer;
	game::Rect sourceRect;
	bool invertX;
	bool invertY;

public:
	static std::vector<RenderData> vRenderList;

	void Add(game::Float2 location, game::Float2 size, int bitmapIndex, int layer, game::Rect sourceRect, bool invertX, bool invertY)
	{
		vRenderList.push_back(RenderData(location, size, bitmapIndex, layer, sourceRect, invertX, invertY));
	}

	game::Rect GetRect(Camera* pCamera)
	{
		if (pCamera)
		{
			game::Rect rect{
				pCamera->WorldTransformToScreenRect(location, size)
			};
			return rect;
		}
		else
		{
			game::Rect rect{
				//location.x * pixels_per_screen_unit - size.x,
				//location.y * pixels_per_screen_unit - size.y,
				//location.x * pixels_per_screen_unit + size.x,
				//location.y * pixels_per_screen_unit + size.y
			};
			return rect;
		}
	}
};

class Sprite
{
protected:
	GameObject* pOwner;
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
	static float pixels_per_world_unit;
	static float pixels_per_screen_unit;
	static std::vector<Sprite*> vpSpritesToRender;

	bool visible;
	bool active;


	Sprite(GameObject* pOwner, ImageData* pImageData, float frameTime, int layer = SL_DEFAULT, game::Float2 scale = { 1,1 }, game::Float2 offset = { 0,0 });
	Sprite(GameObject* pOwner, const Sprite& sprite);

	void Update(float deltaTime);

	void Pause();
	void PlayForwards();
	void PlayBackwards();

	GameObject* GetOwner();

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
	static bool Obstructing(Sprite* pA, Sprite* pB);
};
