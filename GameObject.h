#pragma once

#include "DataTypes.h"
#include "Textures.h"
#include "Sprite.h"


class GameObject
{
public:
	game::Float2 m_location;

	Sprite m_sprite;

	virtual void Update();

	GameObject();

	GameObject(
		game::Float2 location,
		int sprite_layer,
		int sprite_texture,
		game::Float2 sprite_size,
		game::Float2 sprite_offset
	);

	GameObject(const game::Float2 location, const Sprite& sprite);

	static bool CompareRenderLayer(GameObject* gameObjectA, GameObject* gameObjectB);
	static bool CompareLocationX(GameObject* gameObjectA, GameObject* gameObjectB);
	static bool CompareLocationY(GameObject* gameObjectA, GameObject* gameObjectB);


};

