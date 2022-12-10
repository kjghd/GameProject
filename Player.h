#pragma once

#include "DataTypes.h"
#include "Textures.h"
#include "Collider.h"
#include "Sprite.h"


class Player
{
public:
	game::Float2 m_location;
	Sprite m_sprite;
	Collider_Circle m_collider;

	float m_speed;

	Player();

	void Update(float deltaTime);

	void Move(game::Float2 direction, float deltaTime);
};
