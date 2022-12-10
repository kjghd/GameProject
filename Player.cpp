#include "Player.h"

#include <cmath>
#include <algorithm>


Player::Player()
	:
	m_location({ 0,0 }),
	m_sprite(&m_location, T_Wallace, { 1, 1.5 }, { 0, 0.3334 }),
	m_collider(Collider_Circle(&m_location, .5, true, true)),
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
