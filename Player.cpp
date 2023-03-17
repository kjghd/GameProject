#include "Player.h"

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

void Player::Update(float deltaTime)
{
	m_collider.Update();
	m_sprite.Update(deltaTime);

	if (m_pInput)
	{
		if (m_pInput->CheckHeld(BTN_Q)) m_camera.DecreaseZoom(.1);
		if (m_pInput->CheckHeld(BTN_E)) m_camera.IncreaseZoom(.1);

		if (m_pInput->CheckHeld(BTN_W)) Move({ 0, 1 }, deltaTime);
		if (m_pInput->CheckHeld(BTN_S)) Move({ 0,-1 }, deltaTime);
		if (m_pInput->CheckHeld(BTN_A)) Move({ -1, 0 }, deltaTime);
		if (m_pInput->CheckHeld(BTN_D)) Move({ 1, 0 }, deltaTime);
	}

	m_camera.OffsetTo(m_sprite.GetLocation(), deltaTime);

}