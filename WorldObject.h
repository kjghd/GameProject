#pragma once

#include "GameObject.h"


class WorldObject : public GameObject
{
public:
	virtual void Update(float deltaTime) override;

	WorldObject(
		ImageData* sprite_pImageData,
		int sprite_layer = SL_DEFAULT,
		game::Float2 sprite_scale = {1.f,1.f},
		game::Float2 sprite_offset = {0.f,0.f},
		float sprite_frameTime = 1000.f
	);
	WorldObject(const WorldObject& worldObject);
	WorldObject(const Sprite& sprite);

	static bool CompareRenderOrder_Under(WorldObject* gameObjectA, WorldObject* gameObjectB);
	static bool CompareRenderOrder_Above(WorldObject* gameObjectA, WorldObject* gameObjectB);
	static bool CompareRenderOrder_RowAndLeftOf(WorldObject* gameObjectA, WorldObject* gameObjectB);
};

