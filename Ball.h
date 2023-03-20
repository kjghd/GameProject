#pragma once

#include "DataTypes.h"
#include "Textures.h"
#include "Sprite.h"
#include "Collider.h"
#include "GameObject.h"

#include <memory>


class Ball : public GameObject
{
public:
	Collider_Circle m_collider;
	
	virtual void Update(float deltaTime) override;

	Ball();
	Ball(
		ImageData* sprite_pImageData,
		int sprite_layer,
		game::Float2 sprite_scale,
		game::Float2 sprite_offset,
		float collider_radius,
		bool collider_dynamic,
		bool collider_block,
		float sprite_frameTime = 1000.f
	);
	Ball(const Ball& ball);
	Ball(const Sprite& sprite, const Collider_Circle& collider);
};

