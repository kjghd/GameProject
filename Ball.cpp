#include "Ball.h"


Ball::Ball()
	:
	GameObject(),
	m_collider(&m_location)
{
}

Ball::Ball(
	game::Float2 location,
	int sprite_layer,
	int sprite_texture,
	game::Float2 sprite_size,
	game::Float2 sprite_offset,
	float collider_radius,
	bool collider_dynamic,
	bool collider_block
)
	:
	GameObject(location, sprite_layer, sprite_texture, sprite_size, sprite_offset),
	m_collider(&m_location, collider_radius, collider_dynamic, collider_block)
{
}

Ball::Ball(const Ball& ball)
	:
	GameObject(ball.m_location, ball.m_sprite),
	m_collider(&m_location, ball.m_collider)
{
}

void Ball::Update()
{
	m_collider.Update();
}