#include "Player.h"
#include "Input.h"
#include <cmath>


Player::Player(float health,
	float speed,
	ImageData* sprite_pImageData,
	int sprite_layer,
	game::float2 sprite_scale,
	game::float2 sprite_offset,
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
	m_camera(this)
{
	m_tag = "WOPC";
}
Player::Player(const Player& player)
	:
	Character(player.m_sprite, player.m_collider, player.m_viewRange, player.m_health, player.m_speed),
	m_camera(this)
{
	m_tag = "WOPC";
}
Player::Player(std::istream& is)
	:
	Character(is),
	m_camera(this, is)
{
	m_tag = "WOPC";
}

void Player::Update(float deltaTime)
{	
	if (m_health > 0)
	{
		if (Input::GetScrollDistance())
			m_camera.IncreaseZoom(Input::GetScrollDistance() * .4f);

		m_lookDirection = m_camera.ScreenLocToWorldLoc(Input::GetMouseLoc().x, Input::GetMouseLoc().y) - m_location;

		game::float2 direction{ 0,0 };

		if (Input::CheckHeld(BTN_W)) direction.y += 1.f;
		if (Input::CheckHeld(BTN_S)) direction.y -= 1.f;
		if (Input::CheckHeld(BTN_A)) direction.x -= 1.f;
		if (Input::CheckHeld(BTN_D)) direction.x += 1.f;
		if (direction.x != 0 && direction.y != 0)
		{
			direction.x *= std::sqrtf(2.f) / 2.f;
			direction.y *= std::sqrtf(2.f) / 2.f;
		}

		if (direction.x || direction.y)
		{
			Move(direction);
		}

		ApplyMovement(deltaTime);
	}
	else if(m_sprite.GetAnimName() != "dead")
		m_sprite.SetAnimation("dead");

	m_camera.OffsetTo(m_sprite.GetLocation(), deltaTime);

	m_collider.Update();
	m_viewRange.Update();
	m_sprite.Update(deltaTime);
}

GameObject* Player::Clone()
{
	return new Player(*this);
}

void Player::WriteData(std::ostream& os)
{
	Character::WriteData(os);
	os << ',';
	m_camera.WriteData(os);
}
