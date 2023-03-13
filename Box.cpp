#include "Box.h"


Box::Box()
	:
	GameObject(),
	m_collider(&m_location)
{
}


Box::Box(game::Float2 location,
	int sprite_layer,
	int sprite_texture,
	game::Float2 sprite_size,
	game::Float2 sprite_offset,
	game::Float2 collider_size,
	bool collider_dynamic,
	bool collider_block)
	:
	GameObject(location, sprite_layer, sprite_texture, sprite_size, sprite_offset),
	m_collider(&m_location, collider_size, collider_dynamic, collider_block)
{
}

Box::Box(const Box& box)
	:
	GameObject(box.m_location, box.m_sprite),
	m_collider(&m_location, box.m_collider)
{
}

void Box::Update(float deltaTime)
{
	m_collider.Update();
}