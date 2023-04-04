#pragma once

#include "DataTypes.h"
#include "Textures.h"
#include "Sprite.h"


class WorldObject
{
public:
	game::Float2 m_location;

	Sprite m_sprite;

	virtual void Update(float deltaTime);

	WorldObject();

	WorldObject(
		ImageData* sprite_pImageData,
		int sprite_layer,
		game::Float2 sprite_scale,
		game::Float2 sprite_offset,
		float sprite_frameTime = 1000.f
	);

	WorldObject(const WorldObject& gameObject);

	WorldObject(const Sprite& sprite);

	static bool CompareRenderOrder_Under(WorldObject* gameObjectA, WorldObject* gameObjectB);
	static bool CompareRenderOrder_Above(WorldObject* gameObjectA, WorldObject* gameObjectB);
	static bool CompareRenderOrder_RowAndLeftOf(WorldObject* gameObjectA, WorldObject* gameObjectB);
};

