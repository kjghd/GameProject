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
		float sprite_frameTime,
		int sprite_layer,
		game::Float2 sprite_scale,
		game::Float2 sprite_offset,
		float collider_radius,
		bool collider_dynamic,
		bool collider_block
	);
	Ball(const Ball& ball);

	Ball& operator =(const Ball& ball)
	{
		m_location = ball.m_location;
		m_sprite = Sprite(&m_location, ball.m_sprite);
		m_collider = Collider_Circle(&m_location, ball.m_collider);

		return *this;
	}
};

