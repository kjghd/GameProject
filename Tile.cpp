#include "Tile.h"

Tile::Tile()
	:
	m_location{ 0,0 },
	m_sprite(&m_location),
	m_collider(&m_location)
{
}


Tile::Tile(game::Float2 location, int texture, game::Float2 size, game::Float2 offset, bool dynamic, bool block)
	:
	m_location(location),
	m_sprite(Sprite(&m_location, texture, size, offset)),
	m_collider(Collider_Box(&m_location, { 1,1 }, dynamic, block))
{
}

Tile::Tile(const Tile& tile)
	:
	m_location(tile.m_location),
	m_sprite(Sprite(&m_location, tile.m_sprite)),
	m_collider(Collider_Box(&m_location, tile.m_collider)
	)
{
}
