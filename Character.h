#pragma once

#include "Ball.h"


class Character : public Ball
{
protected:
	game::float2 m_moveBuffer;

	virtual void WriteData(std::ostream& os) override;

public:
	float m_health;
	float m_speed;

	game::float2 m_lookDirection;

	Collider_Circle m_viewRange;

	Character(
		float health,
		float speed,
		ImageData* sprite_pImageData,
		int sprite_layer,
		game::float2 sprite_scale,
		game::float2 sprite_offset,
		float collider_radius,
		bool collider_dynamic,
		bool collider_block,
		float view_radius,
		float sprite_frameTime = 1000.f
	);
	Character(const Character& character);
	Character(const Sprite& sprite, const Collider_Circle& collider, const Collider_Circle& colliderView, float health, float speed);
	Character(std::istream& is);

	virtual void Update(float deltaTime) override;

	void Move(game::float2 direction);

	void ApplyMovement(float deltaTime);

	virtual GameObject* Clone() override;

};
