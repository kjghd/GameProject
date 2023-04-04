#include "WorldObject.h"


WorldObject::WorldObject()
	:
	m_location{ 0,0 },
	m_sprite(Sprite(&m_location, nullptr, 0))
{
}

// Used in prefabs
WorldObject::WorldObject(
	ImageData* sprite_pImageData,
	int sprite_layer,
	game::Float2 sprite_scale,
	game::Float2 sprite_offset,
	float sprite_frameTime
)
	:
	m_location{ 0,0 },
	m_sprite(Sprite(&m_location, sprite_pImageData, sprite_frameTime, sprite_layer, sprite_scale, sprite_offset))
{
}

// Used in scene
WorldObject::WorldObject(const WorldObject& gameObject)
	:
	m_location{ 0,0 },
	m_sprite(Sprite(&m_location, gameObject.m_sprite))
{
}

// Used by derived classes
WorldObject::WorldObject(const Sprite& sprite)
	:
	m_location{ 0,0 },
	m_sprite(Sprite(&m_location, sprite))
{
}

void WorldObject::Update(float deltaTime)
{
	m_sprite.Update(deltaTime);
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