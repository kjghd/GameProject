#include "Ball.h"


Ball::Ball()
	:
	GameObject(),
	m_collider(&m_location)
{
}

Ball::Ball(
	ImageData* sprite_pImageData,
	float sprite_frameTime,
	int sprite_layer,
	game::Float2 sprite_scale,
	game::Float2 sprite_offset,
	float collider_radius,
	bool collider_dynamic,
	bool collider_block
)
	:
	GameObject(sprite_pImageData, sprite_frameTime, sprite_layer, sprite_scale, sprite_offset),
	m_collider(&m_location, collider_radius, collider_dynamic, collider_block)
{
}

Ball::Ball(const Ball& ball)
	:
	GameObject(ball.m_sprite),
	m_collider(&m_location, ball.m_collider)
{
}

void Ball::Update(float deltaTime)
{
	m_collider.Update();
	m_sprite.Update(deltaTime);
}