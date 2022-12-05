#pragma once
#include "DataTypes.h"
#include "Textures.h"

class Tile
{
public:
	int m_texture;
	game::Float2 m_location;
	static game::Float2 m_size;
	bool m_block;
	int m_rotation;

	Tile(int texture = T_Error, game::Float2 location = { 0,0 }, bool block = false);
};
