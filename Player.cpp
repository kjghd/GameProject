#include "Player.h"
#include <cmath>

Player::Player(float speed,
	ImageData* sprite_pImageData,
	float sprite_frameTime,
	int sprite_layer,
	game::Float2 sprite_scale,
	game::Float2 sprite_offset,
	float collider_radius,
	bool collider_dynamic,
	bool collider_block)
	:
	Character(speed,
		sprite_pImageData,
		sprite_frameTime,
		sprite_layer,
		sprite_scale,
		sprite_offset,
		collider_radius,
		collider_dynamic,
		collider_block),
	m_pInput(nullptr),
	m_camera(&m_location)
{
}

Player::Player(float speed,
	ImageData* sprite_pImageData,
	int sprite_layer,
	game::Float2 sprite_scale,
	game::Float2 sprite_offset,
	float collider_radius,
	bool collider_dynamic,
	bool collider_block)
	:
	Character(speed,
		sprite_pImageData,
		sprite_layer,
		sprite_scale,
		sprite_offset,
		collider_radius,
		collider_dynamic,
		collider_block),
	m_pInput(nullptr),
	m_camera(&m_location)
{
}

void Player::Update(float deltaTime)
{
	m_collider.Update();
	m_sprite.Update(deltaTime);
	
	if (m_pInput)
	{
		if (m_pInput->CheckHeld(BTN_Q)) m_camera.DecreaseZoom(.1);
		if (m_pInput->CheckHeld(BTN_E)) m_camera.IncreaseZoom(.1);

		game::Float2 moveBuffer{ 0,0 };

		if (m_pInput->CheckHeld(BTN_W)) moveBuffer.y += 1;
		if (m_pInput->CheckHeld(BTN_S)) moveBuffer.y -= 1;
		if (m_pInput->CheckHeld(BTN_A)) moveBuffer.x -= 1;
		if (m_pInput->CheckHeld(BTN_D)) moveBuffer.x += 1;

		if (moveBuffer.x != 0 && moveBuffer.y != 0)
		{
			moveBuffer.x *= std::sqrt(2) / 2;
			moveBuffer.y *= std::sqrt(2) / 2;
		}
		
		if (moveBuffer.x == 0 && moveBuffer.y == 0 && m_sprite.GetCurrentAnimation() != 0)
		{
			m_sprite.SetAnimation(0);
		}
		else
		{
			if (moveBuffer.y > 0 && moveBuffer.x == 0 && m_sprite.GetCurrentAnimation() != 3) m_sprite.SetAnimation(3);
			else if (moveBuffer.y < 0 && moveBuffer.x == 0 && m_sprite.GetCurrentAnimation() != 2) m_sprite.SetAnimation(2);

			if (moveBuffer.x > 0 && m_sprite.GetCurrentAnimation() != 4) m_sprite.SetAnimation(4);
			else if (moveBuffer.x < 0 && m_sprite.GetCurrentAnimation() != 5) m_sprite.SetAnimation(5);

			Move(moveBuffer, deltaTime);
		}
	}

	m_camera.OffsetTo(m_sprite.GetLocation(), deltaTime);

}