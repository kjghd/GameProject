#include "Box.h"


Box::Box(ImageData* sprite_pImageData,
	int sprite_layer,
	game::float2 sprite_scale,
	game::float2 sprite_offset,
	game::float2 collider_size,
	bool collider_dynamic,
	bool collider_block,
	float sprite_frameTime)
	:
	WorldObject(sprite_pImageData, sprite_layer, sprite_scale, sprite_offset, sprite_frameTime),
	m_collider(this, collider_size, collider_dynamic, collider_block)
{
}

Box::Box(const Box& box)
	:
	WorldObject(box.m_sprite),
	m_collider(this, box.m_collider)
{
}

Box::Box(const Sprite& sprite, const Collider_Box& collider)
	:
	WorldObject(sprite),
	m_collider(this, collider)
{
}

void Box::Update(float deltaTime)
{
	m_collider.Update();
	m_sprite.Update(deltaTime);
}

std::string Box::Serialise()
{
	std::string str;

	//str += WorldObject::Serialise();
	//str += m_collider.Serialise();

	return str;
}