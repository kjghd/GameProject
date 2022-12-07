#include "Tile.h"

game::Float2 Tile::m_size = { 1,1 };

Tile::Tile(int texture, game::Float2 location, bool dynamic, bool block)
	:
	m_location(location),
	m_texture(texture),
	m_collider(Collider_Box(&m_location, { 1,1 }, dynamic, block))
{
}

Tile::Tile(const Tile& tile)
	:
	m_location(tile.m_location),
	m_texture(tile.m_texture),
	m_collider(Collider_Box(&m_location, tile.m_collider)
	)
{
}
