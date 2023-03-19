#pragma once

#include "DataTypes.h"
#include "Textures.h"
#include "Sprite.h"


class GameObject
{
public:
	game::Float2 m_location;

	Sprite m_sprite;

	virtual void Update(float deltaTime);

	GameObject();

	GameObject(
		ImageData* sprite_pImageData,
		float sprite_frameTime,
		int sprite_layer,
		game::Float2 sprite_scale,
		game::Float2 sprite_offset
	);

	GameObject(
		ImageData* sprite_pImageData,
		int sprite_layer,
		game::Float2 sprite_scale,
		game::Float2 sprite_offset
	);

	GameObject(const GameObject& gameObject);

	GameObject(const Sprite& sprite);

	GameObject& operator=(const GameObject& gameObject)
	{
		m_location = gameObject.m_location;
		m_sprite = Sprite(&m_location, gameObject.m_sprite);

		return *this;
	}

	static bool CompareRenderOrder_Under(GameObject* gameObjectA, GameObject* gameObjectB);
	static bool CompareRenderOrder_Above(GameObject* gameObjectA, GameObject* gameObjectB);
	static bool CompareRenderOrder_RowAndLeftOf(GameObject* gameObjectA, GameObject* gameObjectB);
};

