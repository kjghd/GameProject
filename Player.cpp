#include "Player.h"

#include <cmath>
#include <algorithm>

Player::Player()
	:
	m_location({ 0,0 }),
	m_spriteOffset({ 0,.5 }),
	m_size({ 1,1.5 }),
	m_collisionSize({ .8,.8 }),
	m_collisionRadius(.4),
	m_texture(T_Shadow),
	m_speed(5)
{
}

void Player::Update(float deltaTime)
{

}

void Player::Move(game::Float2 direction, float deltaTime)
{
	m_location += direction * (m_speed * deltaTime / 1000.f);
}

void Player::Collide_BoxVsBox(Tile& tile)
{
	// Object A (this)
	float at = m_location.y + m_collisionSize.y / 2;
	float ab = m_location.y - m_collisionSize.y / 2;
	float al = m_location.x - m_collisionSize.x / 2;
	float ar = m_location.x + m_collisionSize.x / 2;
	// Object B (target)
	float bt = tile.m_location.y + tile.m_size.y / 2;
	float bb = tile.m_location.y - tile.m_size.y / 2;
	float bl = tile.m_location.x - tile.m_size.x / 2;
	float br = tile.m_location.x + tile.m_size.x / 2;

	// Overlap
	float t = at - bb;
	float b = bt - ab;
	float l = br - al;
	float r = ar - bl;
	// Shortest overlap
	float shortest_y = t < b ? t : b;
	float shortest_x = l < r ? l : r;
	float shortest = shortest_x < shortest_y ? shortest_x : shortest_y;

	if (shortest >= 0)
	{
		if (tile.m_block)
		{
			if (t == shortest) m_location.y -= t;
			if (b == shortest) m_location.y += b;
			if (l == shortest) m_location.x += l;
			if (r == shortest) m_location.x -= r;
		}
	}
}
void Player::Collide_CircleVsBox(Tile& tile)
{
	game::Float2 point{
			std::clamp<float>(
				m_location.x,
				tile.m_location.x - tile.m_size.x / 2,
				tile.m_location.x + tile.m_size.x / 2
			),
			std::clamp<float>(
				m_location.y,
				tile.m_location.y - tile.m_size.y / 2,
				tile.m_location.y + tile.m_size.y / 2
			)
	};

	game::Float2 vec{ point - m_location };

	float length = std::sqrtf(std::powf(vec.x, 2.f) + std::powf(vec.y, 2.f));
	float overlap = m_collisionRadius - length;

	if (overlap >= 0)
	{
		if (tile.m_block)
		{
			m_location -= vec * (overlap / length);
		}
	}
}
