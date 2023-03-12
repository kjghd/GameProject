#include "Tile.h"


Tile::Tile()
	:
	GameObject(),
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
	GameObject(location, sprite_layer, sprite_texture, sprite_size, sprite_offset),
	m_collider(&m_location, collider_size, collider_dynamic, collider_block)
{
}

Tile::Tile(const Tile& tile)
	:
	GameObject(tile.m_location, tile.m_sprite),
	m_collider(&m_location, tile.m_collider)
{
}

void Tile::Update(float deltaTime)
{
	m_collider.Update();
}