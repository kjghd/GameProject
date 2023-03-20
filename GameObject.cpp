#include "GameObject.h"


GameObject::GameObject()
	:
	m_location{ 0,0 },
	m_sprite(Sprite(&m_location, nullptr, 0))
{
}

// Used in prefabs
GameObject::GameObject(
	ImageData* sprite_pImageData,
	int sprite_layer,
	game::Float2 sprite_scale,
	game::Float2 sprite_offset,
	float sprite_frameTime
)
	:
	m_location{ 0,0 },
	m_sprite(Sprite(&m_location, sprite_pImageData, sprite_frameTime, sprite_layer, sprite_scale, sprite_offset))
{
}

// Used in scene
GameObject::GameObject(const GameObject& gameObject)
	:
	m_location{ 0,0 },
	m_sprite(Sprite(&m_location, gameObject.m_sprite))
{
}

// Used by derived classes
GameObject::GameObject(const Sprite& sprite)
	:
	m_location{ 0,0 },
	m_sprite(Sprite(&m_location, sprite))
{
}

void GameObject::Update(float deltaTime)
{
	m_sprite.Update(deltaTime);
}

bool GameObject::CompareRenderOrder_Under(GameObject* gameObjectA, GameObject* gameObjectB)
{
	return gameObjectA->m_sprite.GetRenderLayer() > gameObjectB->m_sprite.GetRenderLayer();
}

bool GameObject::CompareRenderOrder_Above(GameObject* gameObjectA, GameObject* gameObjectB)
{
	return gameObjectA->m_location.y > gameObjectB->m_location.y;
}

bool GameObject::CompareRenderOrder_RowAndLeftOf(GameObject* gameObjectA, GameObject* gameObjectB)
{
	return gameObjectA->m_location.y == gameObjectB->m_location.y && gameObjectA->m_location.x < gameObjectB->m_location.x;
}