#include "Tile.h"


Tile::Tile()
	:
	m_location{ 0,0 },
	m_sprite(&m_location),
	m_collider(&m_location)
{
}


Tile::Tile(game::Float2 location,
	int sprite_layer,
	int sprite_texture,
	game::Float2 sprite_size,
	game::Float2 sprite_offset,
	game::Float2 collider_size,
	bool collider_dynamic,
	bool collider_block)
	:
	m_location(location),
	m_sprite(Sprite(&m_location, sprite_layer, sprite_texture, sprite_size, sprite_offset)),
	m_collider(Collider_Box(&m_location, collider_size, collider_dynamic, collider_block))
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

bool Tile::CompareRenderLayer(Tile& tileA, Tile& tileB)
{
	return tileA.m_sprite.layer < tileB.m_sprite.layer;
}