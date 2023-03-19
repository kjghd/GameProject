#include "Box.h"


Box::Box()
	:
	GameObject(),
	m_collider(&m_location)
{
}


Box::Box(ImageData* sprite_pImageData,
	float sprite_frameTime,
	int sprite_layer,
	game::Float2 sprite_scale,
	game::Float2 sprite_offset,
	game::Float2 collider_size,
	bool collider_dynamic,
	bool collider_block)
	:
	GameObject(sprite_pImageData, sprite_frameTime, sprite_layer, sprite_scale, sprite_offset),
	m_collider(&m_location, collider_size, collider_dynamic, collider_block)
{
}

Box::Box(ImageData* sprite_pImageData,
	int sprite_layer,
	game::Float2 sprite_scale,
	game::Float2 sprite_offset,
	game::Float2 collider_size,
	bool collider_dynamic,
	bool collider_block)
	:
	GameObject(sprite_pImageData, sprite_layer, sprite_scale, sprite_offset),
	m_collider(&m_location, collider_size, collider_dynamic, collider_block)
{
}

Box::Box(const Box& box)
	:
	GameObject(box.m_sprite),
	m_collider(&m_location, box.m_collider)
{
}

void Box::Update(float deltaTime)
{
	m_collider.Update();
	m_sprite.Update(deltaTime);
}