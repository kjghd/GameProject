#include "Player.h"

Player::Player()
	:
	Character(),
	m_pInput(nullptr),
	m_camera(&m_location)
{
}

void Player::Update(float deltaTime)
{
	m_collider.Update();

	if (m_pInput)
	{
		if (m_pInput->CheckHeld(BTN_Q)) m_camera.DecreaseZoom(2);
		if (m_pInput->CheckHeld(BTN_E)) m_camera.IncreaseZoom(2);

		if (m_pInput->CheckHeld(BTN_W)) Move({ 0, 1 }, deltaTime);
		if (m_pInput->CheckHeld(BTN_S)) Move({ 0,-1 }, deltaTime);
		if (m_pInput->CheckHeld(BTN_A)) Move({ -1, 0 }, deltaTime);
		if (m_pInput->CheckHeld(BTN_D)) Move({ 1, 0 }, deltaTime);
	}

	game::Float2 playerCenter{
	m_location.x + m_sprite.offset.x,
	m_location.y + m_sprite.offset.y
	};
	m_camera.OffsetTo(playerCenter, deltaTime);

}