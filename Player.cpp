#include "Player.h"

#include <cmath>
#include <algorithm>

Player::Player()
	:
	m_location({ 0,0 }),
	m_spriteOffset({ 0,.5 }),
	m_size({ 1,1.5 }),
	m_texture(T_Shadow),
	m_speed(5),
	m_collider(Collider_Circle(&m_location, .5, true, true))
{
}

void Player::Update(float deltaTime)
{

}

void Player::Move(game::Float2 direction, float deltaTime)
{
	m_location += direction * (m_speed * deltaTime / 1000.f);
}
