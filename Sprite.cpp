#include "Sprite.h"


Sprite::Sprite(game::Float2* pOrigin, int layer, int texture, game::Float2 size, game::Float2 offset)
	:
	origin(*pOrigin),
	layer(layer),
	texture(texture),
	size(size),
	offset(offset)
{
}

Sprite::Sprite(game::Float2* pOrigin, const Sprite& sprite)
	:
	origin(*pOrigin),
	layer(sprite.layer),
	texture(sprite.texture),
	size(sprite.size),
	offset(sprite.offset)
{
}