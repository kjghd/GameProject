#pragma once

#include "GameObject.h"
#include "Collider.h"


class Box : public GameObject
{
public:
	Collider_Box m_collider;

	virtual void Update(float deltaTime) override;

	Box();
	Box(
		game::Float2 location,
		int sprite_layer,
		int sprite_texture,
		game::Float2 sprite_size,
		game::Float2 sprite_offset,
		game::Float2 collider_size,
		bool collider_dynamic,
		bool collider_block
	);
	Box(const Box& box);

	Box& operator =(const Box& box)
	{
		m_location = box.m_location;
		m_sprite = Sprite(&m_location, box.m_sprite);
		m_collider = Collider_Box(&m_location, box.m_collider);
		return *this;
	}
};
