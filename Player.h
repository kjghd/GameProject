#pragma once
#include "DataTypes.h"
#include "Textures.h"
#include "Tile.h"

class Player
{
public:
	game::Float2 m_location;
	game::Float2 m_spriteOffset;
	game::Float2 m_size;
	game::Float2 m_collisionSize;
	float m_collisionRadius;
	int m_texture;
	float m_speed;
	float m_health;

	Player();

	void Update(float deltaTime);

	void Move(game::Float2 direction, float deltaTime);

	void Collide_BoxVsBox(Tile& tile);
	void Collide_CircleVsBox(Tile& tile);
};
