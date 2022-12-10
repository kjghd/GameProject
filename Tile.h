#pragma once

#include "DataTypes.h"
#include "Textures.h"
#include "Sprite.h"
#include "Collider.h"

#include <memory>


class Tile
{
public:
	game::Float2 m_location;

	Sprite m_sprite;
	Collider_Box m_collider;

	Tile();
	Tile(game::Float2 location, int sprite_texture, game::Float2 sprite_size, game::Float2 sprite_offset, bool collider_dynamic, bool collider_block);
	Tile(const Tile& tile);

	Tile& operator =(const Tile& tile)
	{
		m_location = tile.m_location;
		m_sprite = Sprite(&m_location, tile.m_sprite),
		m_collider = Collider_Box(&m_location, tile.m_collider);
		return *this;
	}
};
