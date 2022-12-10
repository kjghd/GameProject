#pragma once
#include "DataTypes.h"
#include "Textures.h"

class Sprite
{
public:
	game::Float2& origin;

	int texture;
	game::Float2 size;
	game::Float2 offset;

	Sprite(game::Float2* pOrigin, int tex = T_Error, game::Float2 sz = {1,1}, game::Float2 off = {0,0});
	Sprite(game::Float2* pOrigin, const Sprite& sprite);

	Sprite& operator =(const Sprite& sprite)
	{
		origin = sprite.origin;
		texture = sprite.texture;
		size = sprite.size;
		offset = sprite.offset;
		return *this;
	}
};
