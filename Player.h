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
		float speed,
		ImageData* sprite_pImageData,
		float sprite_frameTime,
		int sprite_layer,
		game::Float2 sprite_scale,
		game::Float2 sprite_offset,
		float collider_radius,
		bool collider_dynamic,
		bool collider_block
	);

    virtual void Update(float deltaTime) override;

	Player& operator =(const Player& player)
	{
		m_location = player.m_location;
		m_sprite = Sprite(&m_location, player.m_sprite);
		m_collider = Collider_Circle(&m_location, player.m_collider);
		m_speed = player.m_speed;
		m_pInput = player.m_pInput;
		m_camera = m_camera;

		return *this;
	}
};

