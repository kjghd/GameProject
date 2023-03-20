#include "Character.h"

#include <cmath>
#include <algorithm>


Character::Character(
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
	float sprite_frameTime
)
	:
	Ball(sprite_pImageData,
		sprite_layer,
		sprite_scale,
		sprite_offset,
		collider_radius,
		collider_dynamic,
		collider_block,
		sprite_frameTime),
	m_health(health),
	m_speed(speed),
	m_viewRange(this, view_radius, false, false, true)
{
}

Character::Character(const Character& character)
	:
	Ball(character.m_sprite, character.m_collider),
	m_viewRange(this, character.m_viewRange),
	m_health(character.m_health),
	m_speed(character.m_speed)
{
}

Character::Character(const Sprite& sprite, const Collider_Circle& collider, const Collider_Circle& colliderView, float health, float speed)
	:
	Ball(sprite, collider),
	m_viewRange(this, colliderView),
	m_health(health),
	m_speed(speed)
{
}

void Character::Update(float deltaTime)
{
	ApplyMovement(deltaTime);
	m_collider.Update();
	m_viewRange.Update();
	m_sprite.Update(deltaTime);
}

void Character::Move(game::Float2 direction)
{
	m_moveBuffer += direction;
}

void Character::ApplyMovement(float deltaTime)
{
	if (m_moveBuffer.x != 0 || m_moveBuffer.y != 0)
	{
		game::Float2 direction{
			std::clamp<float>(m_moveBuffer.x, -1.f, 1.f),
			std::clamp<float>(m_moveBuffer.y, -1.f, 1.f)
		};
		m_moveBuffer = { 0,0 };

		float u{ direction.y > 0 ? direction.y : 0 };
		float d{ direction.y < 0 ? direction.y : 0 };
		float l{ direction.x < 0 ? direction.x : 0 };
		float r{ direction.x > 0 ? direction.x : 0 };

		float x{ r > -l ? r : -l };
		float y{ u > -d ? u : -d };


		// Up
		if (u > -d && (u >= x && x < y) && m_sprite.GetCurrentAnimation() != 4)
			m_sprite.SetAnimation(4);

		// Down
		else if (u < -d && (d <= -x && x < y) && m_sprite.GetCurrentAnimation() != 3)
			m_sprite.SetAnimation(3);

		// Left
		else if (r < -l && (l <= -y || x == y) && m_sprite.GetCurrentAnimation() != 6)
			m_sprite.SetAnimation(6);

		// Right
		else if (r > -l && (r >= y || x == y) && m_sprite.GetCurrentAnimation() != 5)
			m_sprite.SetAnimation(5);


		m_location += direction * (m_speed * deltaTime / 1000.f);
		m_collider.moving = true;
	}
	else
		m_sprite.SetAnimation(0);
}
