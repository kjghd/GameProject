#include "Character.h"

Character::Character(
	float speed,
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
	Ball(sprite_pImageData,
		sprite_frameTime,
		sprite_layer,
		sprite_scale,
		sprite_offset,
		collider_radius,
		collider_dynamic,
		collider_block),
	m_speed(speed)
{
}

void Character::Update(float deltaTime)
{
	m_collider.Update();
	m_sprite.Update(deltaTime);
}

void Character::Move(game::Float2 direction, float deltaTime)
{
	m_location += direction * (m_speed * deltaTime / 1000.f);
	m_collider.moving = true;
}
