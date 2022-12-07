#pragma once
#include "DataTypes.h"
#include "Textures.h"
#include "Collider.h"
#include <memory>

class Tile
{
public:
	int m_texture;
	game::Float2 m_location;
	static game::Float2 m_size;
	
	Collider_Box m_collider;

	Tile(int texture = T_Error, game::Float2 location = { 0,0 }, bool dynamic = false, bool block = false);
	Tile(const Tile& tile);

	Tile& operator =(const Tile& tile)
	{
		m_texture = tile.m_texture;
		m_location = tile.m_location;
		m_size = tile.m_size;
		m_collider = Collider_Box(&m_location, tile.m_collider);
		return *this;
	}
};
