#pragma once

#include "DataTypes.h"
#include "Sprite.h"

class GameObject
{
public:
	game::Float2 m_location;
	Sprite m_sprite;

	GameObject(
		ImageData* sprite_pImageData,
		int sprite_layer,
		game::Float2 sprite_scale,
		game::Float2 sprite_offset,
		float sprite_frameTime = 1000.f
	);
	GameObject(const GameObject& gameObject);
	GameObject(const Sprite& sprite);

	virtual void Update(float deltaTime);
};