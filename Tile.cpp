#include "Tile.h"

game::Float2 Tile::m_size = { 1,1 };

Tile::Tile(int texture, game::Float2 location, bool block)
	:
	m_location(location),
	m_texture(texture),
	m_block(block)
{
}