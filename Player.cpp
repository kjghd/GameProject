#include "Player.h"
#include <cmath>

Player::Player(float health,
	float speed,
	ImageData* sprite_pImageData,
	int sprite_layer,
	game::Float2 sprite_scale,
	game::Float2 sprite_offset,
	float collider_radius,
	bool collider_dynamic,
	bool collider_block,
	float view_radius,
	float sprite_frameTime)
	:
	Character(
		health,
		speed,
		sprite_pImageData,
		sprite_layer,
		sprite_scale,
		sprite_offset,
		collider_radius,
		collider_dynamic,
		collider_block,
		view_radius,
		sprite_frameTime),
	m_pInput(nullptr),
	m_camera(&m_location)
{
}


Player::Player(const Player& player)
	:
	Character(player.m_sprite, player.m_collider, player.m_viewRange, player.m_health, player.m_speed),
	m_pInput(player.m_pInput),
	m_camera(&m_location)
{
}


void Player::Update(float deltaTime)
{	
	if (m_health > 0)
	{
		if (m_pInput)
		{
			if (m_pInput->CheckHeld(BTN_Q)) m_camera.DecreaseZoom(.1f);
			if (m_pInput->CheckHeld(BTN_E)) m_camera.IncreaseZoom(.1f);

			m_lookDirection = m_camera.ScreenLocToWorldLoc(m_pInput->GetMouseLoc().x, m_pInput->GetMouseLoc().y) - m_location;

			game::Float2 direction{ 0,0 };

			if (m_pInput->CheckHeld(BTN_W)) direction.y += 1.f;
			if (m_pInput->CheckHeld(BTN_S)) direction.y -= 1.f;
			if (m_pInput->CheckHeld(BTN_A)) direction.x -= 1.f;
			if (m_pInput->CheckHeld(BTN_D)) direction.x += 1.f;
			if (direction.x != 0 && direction.y != 0)
			{
				direction.x *= std::sqrtf(2.f) / 2.f;
				direction.y *= std::sqrtf(2.f) / 2.f;
			}

			if (direction.x || direction.y)
			{
				Move(direction);
			}
		}

		ApplyMovement(deltaTime);
	}
	else if(m_sprite.GetCurrentAnimation() != 3)
		m_sprite.SetAnimation(3);

	m_camera.OffsetTo(m_sprite.GetLocation(), deltaTime);

	m_collider.Update();
	m_viewRange.Update();
	m_sprite.Update(deltaTime);
}