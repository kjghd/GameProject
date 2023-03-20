#pragma once

#include "Ball.h"


class Character : public Ball
{
	game::Float2 m_moveBuffer;

public:
	float m_health;
	float m_speed;

	Collider_Circle m_viewRange;

	Character(
		float health,
		float speed,
		ImageData* sprite_pImageData,
		int sprite_layer,
		game::Float2 sprite_scale,
		game::Float2 sprite_offset,
		float collider_radius,
		bool collider_dynamic,
		bool collider_block,
		float view_radius,
		float sprite_frameTime = 1000.f
	);

	Character(const Character& character);

	Character(const Sprite& sprite, const Collider_Circle& collider, const Collider_Circle& colliderView, float health, float speed);


	virtual void Update(float deltaTime) override;

	void Move(game::Float2 direction);

	void ApplyMovement(float deltaTime);

	Character& operator =(const Character& character)
	{
		m_location = character.m_location;
		m_sprite = Sprite(&m_location, character.m_sprite);
		m_collider = Collider_Circle(this, character.m_collider);
		m_speed = character.m_speed;
		m_viewRange = Collider_Circle(this, character.m_viewRange);

		return *this;
	}

};
