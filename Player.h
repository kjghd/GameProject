#pragma once
#include "Character.h"

#include "Input.h"
#include "Camera.h"

class Player : public Character
{
public:
    Input* m_pInput;
    Camera m_camera;

	Player(
		float health,
		float speed,
		ImageData* sprite_pImageData,
		int sprite_layer,
		game::Float2 sprite_scale,
		game::Float2 sprite_offset,
		float collider_radius,
		bool collider_dynamic,
		bool collider_block,
		float view_radius,
		float sprite_frameTime = 1000.f
	);

	Player(const Player& player);

    virtual void Update(float deltaTime) override;

	Player& operator =(const Player& player)
	{
		m_location = player.m_location;
		m_sprite = Sprite(&m_location, player.m_sprite);
		m_collider = Collider_Circle(this, player.m_collider);
		m_speed = player.m_speed;
		m_pInput = player.m_pInput;
		m_camera = m_camera;
		m_viewRange = Collider_Circle(this, player.m_viewRange);

		return *this;
	}
};

