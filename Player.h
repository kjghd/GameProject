#pragma once
#include "DataTypes.h"
#include "Textures.h"
//#include "Tile.h"
#include "Collider.h"

class Player
{
public:
	game::Float2 m_location;
	game::Float2 m_spriteOffset;
	game::Float2 m_size;
	int m_texture;
	float m_speed;
	Collider_Circle m_collider;

	Player();

	void Update(float deltaTime);

	void Move(game::Float2 direction, float deltaTime);
};
