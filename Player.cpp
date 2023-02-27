#include "Player.h"


Player::Player()
	:
	Ball({ 0,0 }, SL_Object, T_Wallace, { 1, 1.5 }, { 0, 0.5 }, .3, true, true),
	m_speed(5)
{
}

void Player::Update(float deltaTime)
{
	m_collider.Update();
}

void Player::Move(game::Float2 direction, float deltaTime)
{
	m_location += direction * (m_speed * deltaTime / 1000.f);
}
