
#pragma once
#include "FileWritable.h"
#include "DataTypes.h"
#include "ImageData.h"
#include "Camera.h"
#include <vector>
#include <string>


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

class Sprite : public FileWritable
{
protected:
	GameObject* pOwner;
	game::float2 offset;
	game::float2 scale;

	int layer;
	ImageData* pImageData;
	int currentFrame;
	int currentAnim;
	float frameTimeMax;
	float frameTimeCurrent;
	int direction;
	bool invertedX;
	bool invertedY;

	//virtual void WriteData(std::ostream& os) override;

public:
	static float pixels_per_world_unit;
	static float pixels_per_screen_unit;
	static std::vector<Sprite*> vpSpritesToRender;

	bool visible;
	bool active;


	Sprite(GameObject* pOwner, ImageData* pImageData, float frameTime, int layer = SL_DEFAULT, game::float2 scale = { 1,1 }, game::float2 offset = { 0,0 });
	Sprite(GameObject* pOwner, const Sprite& sprite);
	Sprite(GameObject* pOwner, std::istream& is);

	void Update(float deltaTime);

	void Pause();
	void PlayForwards();
	void PlayBackwards();

	GameObject* GetOwner();

	game::float2 GetLocation();
	game::float2 GetSize();

	int GetRenderLayer();
	game::rect GetSourceRect();
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

	virtual void WriteData(std::ostream& os) override;
};
