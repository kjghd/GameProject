#pragma once

#include "Ball.h"


class Character : public Ball
{
public:
	float m_speed;

	Character(
		float speed,
		ImageData* sprite_pImageData,
		float sprite_frameTime,
		int sprite_layer,
		game::Float2 sprite_scale,
		game::Float2 sprite_offset,
		float collider_radius,
		bool collider_dynamic,
		bool collider_block
	);

	Character(
		float speed,
		ImageData* sprite_pImageData,
		int sprite_layer,
		game::Float2 sprite_scale,
		game::Float2 sprite_offset,
		float collider_radius,
		bool collider_dynamic,
		bool collider_block
	);

	virtual void Update(float deltaTime) override;

	void Move(game::Float2 direction, float deltaTime);

	Character& operator =(const Character& character)
	{
		m_location = character.m_location;
		m_sprite = Sprite(&m_location, character.m_sprite);
		m_collider = Collider_Circle(&m_location, character.m_collider);
		m_speed = character.m_speed;

		return *this;
	}

};
