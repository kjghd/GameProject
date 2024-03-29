#include "Character.h"

#include <cmath>
#include <algorithm>


enum Animations {
	Anim_Idle_Down,
	Anim_Idle_Up,
	Anim_Idle_Horizontal,
	Anim_Dead,
	Anim_Down,
	Anim_Up,
	Anim_Horizontal
};

Character::Character(
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
	m_viewRange(this, view_radius, false, false, true),
	m_lookDirection{0,0}
{
}

Character::Character(const Character& character)
	:
	Ball(character.m_sprite, character.m_collider),
	m_viewRange(this, character.m_viewRange),
	m_health(character.m_health),
	m_speed(character.m_speed),
	m_lookDirection(character.m_lookDirection)
{
}

Character::Character(const Sprite& sprite, const Collider_Circle& collider, const Collider_Circle& colliderView, float health, float speed)
	:
	Ball(sprite, collider),
	m_viewRange(this, colliderView),
	m_health(health),
	m_speed(speed),
	m_lookDirection{0,0}
{
}

void Character::Update(float deltaTime)
{
	ApplyMovement(deltaTime);
	m_collider.Update();
	m_viewRange.Update();
	m_sprite.Update(deltaTime);
}

void Character::Move(game::float2 direction)
{
	m_moveBuffer += direction;
}

void Character::ApplyMovement(float deltaTime)
{
	if (m_moveBuffer.x != 0 || m_moveBuffer.y != 0)
	{
		game::float2 direction{
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
		if (u > -d && (u >= x && x < y))
		{
			if (m_lookDirection.y > 0)
			{
				if (m_sprite.GetCurrentAnimation() != Anim_Up) m_sprite.SetAnimation(Anim_Up);
				m_sprite.PlayForwards();
			}
			else if (m_lookDirection.y < 0)
			{
				if (m_sprite.GetCurrentAnimation() != Anim_Down) m_sprite.SetAnimation(Anim_Down);
				m_sprite.PlayBackwards();
			}
		}

		// Down
		else if (u < -d && (d <= -x && x < y))
		{
			if (m_lookDirection.y < 0)
			{
				if (m_sprite.GetCurrentAnimation() != Anim_Down) m_sprite.SetAnimation(Anim_Down);
				m_sprite.PlayForwards();
			}
			else if (m_lookDirection.y > 0)
			{
				if (m_sprite.GetCurrentAnimation() != Anim_Up) m_sprite.SetAnimation(Anim_Up);
				m_sprite.PlayBackwards();
			}
		}

		// Left
		else if (r < -l && (l <= -y || x == y))
		{
			if (m_sprite.GetCurrentAnimation() != Anim_Horizontal) m_sprite.SetAnimation(Anim_Horizontal);
			if (m_lookDirection.x < 0)
			{
				if (!m_sprite.CheckInvertedX()) m_sprite.FlipX();
				m_sprite.PlayForwards();
			}
			else if (m_lookDirection.x > 0)
			{
				if (m_sprite.CheckInvertedX()) m_sprite.FlipX();
				m_sprite.PlayBackwards();
			}
		}

		// Right
		else if (r > -l && (r >= y || x == y))
		{
			if (m_sprite.GetCurrentAnimation() != Anim_Horizontal) m_sprite.SetAnimation(Anim_Horizontal);
			if (m_lookDirection.x > 0)
			{
				if (m_sprite.CheckInvertedX()) m_sprite.FlipX();
				m_sprite.PlayForwards();

			}
			else if (m_lookDirection.x < 0)
			{
				if (!m_sprite.CheckInvertedX()) m_sprite.FlipX();
				m_sprite.PlayBackwards();
			}
		}


		m_location += direction * (m_speed * deltaTime / 1000.f);
		m_collider.moving = true;
	}
	else if (m_lookDirection.x != 0 && m_lookDirection.y != 0)
	{
		game::float2 direction{
			std::clamp<float>(m_lookDirection.x, -1.f, 1.f),
			std::clamp<float>(m_lookDirection.y, -1.f, 1.f)
		};
		m_moveBuffer = { 0,0 };
		float u{ direction.y > 0 ? direction.y : 0 };
		float d{ direction.y < 0 ? direction.y : 0 };
		float l{ direction.x < 0 ? direction.x : 0 };
		float r{ direction.x > 0 ? direction.x : 0 };

		float x{ r > -l ? r : -l };
		float y{ u > -d ? u : -d };

		// Up
		if (u > -d && (u >= x && x < y) && m_sprite.GetCurrentAnimation() != Anim_Idle_Up)
			m_sprite.SetAnimation(Anim_Idle_Up);

		// Down
		else if (u < -d && (d <= -x && x < y) && m_sprite.GetCurrentAnimation() != Anim_Idle_Down)
			m_sprite.SetAnimation(Anim_Idle_Down);

		// Left
		else if (r < -l && (l <= -y || x == y) && m_sprite.GetCurrentAnimation() != Anim_Idle_Horizontal)
		{
			m_sprite.SetAnimation(Anim_Idle_Horizontal);
			if (!m_sprite.CheckInvertedX()) m_sprite.FlipX();
		}

		// Right
		else if (r > -l && (r >= y || x == y) && m_sprite.GetCurrentAnimation() != Anim_Idle_Horizontal)
		{
			m_sprite.SetAnimation(Anim_Idle_Horizontal);
			if (m_sprite.CheckInvertedX()) m_sprite.FlipX();
		}
	}
	else if (m_sprite.GetCurrentAnimation() != Anim_Idle_Down)
		m_sprite.SetAnimation(Anim_Idle_Down);
}
	
std::string Character::Serialise()
{
	std::string str;

	//str += Ball::Serialise();
	//str += game::DataToString<float>(m_health);
	//str += game::DataToString<float>(m_speed);
	//str += game::DataToString<game::float2>(m_lookDirection);
	//str += m_viewRange.Serialise();

	return str;
}