#pragma once

#include "GameObject.h"


class WorldObject : public GameObject
{
protected:
	virtual void WriteData(std::ostream& os) override;

public:
	virtual void Update(float deltaTime) override;

	WorldObject(
		ImageData* sprite_pImageData,
		int sprite_layer = SL_DEFAULT,
		game::float2 sprite_scale = {1.f,1.f},
		game::float2 sprite_offset = {0.f,0.f},
		float sprite_frameTime = 1000.f
	);
	WorldObject(const WorldObject& worldObject);
	WorldObject(const Sprite& sprite);
	WorldObject(std::istream& is);

	static bool CompareRenderOrder_Under(WorldObject* gameObjectA, WorldObject* gameObjectB);
	static bool CompareRenderOrder_Above(WorldObject* gameObjectA, WorldObject* gameObjectB);
	static bool CompareRenderOrder_RowAndLeftOf(WorldObject* gameObjectA, WorldObject* gameObjectB);

	virtual GameObject* Clone() override;
};
