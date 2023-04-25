#include "WorldObject.h"


// Used in prefabs
WorldObject::WorldObject(
	ImageData* sprite_pImageData,
	int sprite_layer,
	game::Float2 sprite_scale,
	game::Float2 sprite_offset,
	float sprite_frameTime
)
	:
	GameObject(sprite_pImageData, sprite_layer, sprite_scale, sprite_offset, sprite_frameTime)
{
}

// Used in scene
WorldObject::WorldObject(const WorldObject& worldObject)
	:
	GameObject(worldObject)
{
}

// Used by derived classes
WorldObject::WorldObject(const Sprite& sprite)
	:
	GameObject(sprite)
{
}

bool WorldObject::CompareRenderOrder_Under(WorldObject* gameObjectA, WorldObject* gameObjectB)
{
	return gameObjectA->m_sprite.GetRenderLayer() > gameObjectB->m_sprite.GetRenderLayer();
}
bool WorldObject::CompareRenderOrder_Above(WorldObject* gameObjectA, WorldObject* gameObjectB)
{
	return gameObjectA->m_location.y > gameObjectB->m_location.y;
}
bool WorldObject::CompareRenderOrder_RowAndLeftOf(WorldObject* gameObjectA, WorldObject* gameObjectB)
{
	return gameObjectA->m_location.y == gameObjectB->m_location.y && gameObjectA->m_location.x < gameObjectB->m_location.x;
}

void WorldObject::Update(float deltaTime)
{
	m_sprite.Update(deltaTime);
}