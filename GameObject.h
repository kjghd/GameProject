#pragma once

#include "DataTypes.h"
#include "Sprite.h"
#include <fstream>
class GameObject
{
public:
	game::float2 m_location;
	Sprite m_sprite;

	GameObject(
		ImageData* sprite_pImageData,
		int sprite_layer,
		game::float2 sprite_scale,
		game::float2 sprite_offset,
		float sprite_frameTime = 1000.f
	);
	GameObject(const GameObject& gameObject);
	GameObject(const Sprite& sprite);
	//GameObject(std::ifstream& file);

	virtual void Update(float deltaTime);
	virtual void Update_SpriteOnly();

	virtual std::string Serialise();
};