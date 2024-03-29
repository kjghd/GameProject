#include "Ball.h"


Ball::Ball(
	ImageData* sprite_pImageData,
	int sprite_layer,
	game::float2 sprite_scale,
	game::float2 sprite_offset,
	float collider_radius,
	bool collider_dynamic,
	bool collider_block,
	float sprite_frameTime
)
	:
	WorldObject(sprite_pImageData, sprite_layer, sprite_scale, sprite_offset, sprite_frameTime),
	m_collider(this, collider_radius, collider_dynamic, collider_block)
{
}

Ball::Ball(const Ball& ball)
	:
	WorldObject(ball.m_sprite),
	m_collider(this, ball.m_collider)
{
}

Ball::Ball(const Sprite& sprite, const Collider_Circle& collider)
	:
	WorldObject(sprite),
	m_collider(this, collider)
{
}

void Ball::Update(float deltaTime)
{
	m_collider.Update();
	m_sprite.Update(deltaTime);
}

std::string Ball::Serialise()
{
	std::string str;
   //
	//str += WorldObject::Serialise();
	//str += m_collider.Serialise();
   //
	return str;
}
