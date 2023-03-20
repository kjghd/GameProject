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
		ImageData* sprite_pImageData,
		int sprite_layer,
		game::Float2 sprite_scale,
		game::Float2 sprite_offset,
		game::Float2 collider_size,
		bool collider_dynamic,
		bool collider_block,
		float sprite_frameTime = 1000.f
	);
	Box(const Box& box);

	Box(const Sprite& sprite, const Collider_Box& collider);
};
