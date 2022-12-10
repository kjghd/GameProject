#include "Sprite.h"


Sprite::Sprite(game::Float2* pOrigin, int tex, game::Float2 sz, game::Float2 off)
	:
	origin(*pOrigin),
	texture(tex),
	size(sz),
	offset(off)
{
}

Sprite::Sprite(game::Float2* pOrigin, const Sprite& sprite)
	:
	origin(*pOrigin),
	texture(sprite.texture),
	size(sprite.size),
	offset(sprite.offset)
{
}