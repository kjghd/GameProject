#pragma once

#include "DataTypes.h"
#include "Textures.h"

enum SpriteLayers
{
	SL_Player,
	SL_Dynamic,
	SL_Wall,
	SL_Floor,
	SL_DEFAULT,

	SL_COUNT
};

class Sprite
{
public:
	game::Float2& origin;

	int layer;
	int texture;
	game::Float2 size;
	game::Float2 offset;
	
	Sprite(game::Float2* pOrigin, int layer = SL_DEFAULT, int tex = T_Error, game::Float2 sz = { 1,1 }, game::Float2 off = { 0,0 });
	Sprite(game::Float2* pOrigin, const Sprite& sprite);

	Sprite& operator =(const Sprite& sprite)
	{
		origin = sprite.origin;
		layer = sprite.layer;
		texture = sprite.texture;
		size = sprite.size;
		offset = sprite.offset;
		return *this;
	}
};
