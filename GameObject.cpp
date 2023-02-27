#include "GameObject.h"


GameObject::GameObject()
	:
	m_location{ 0,0 },
	m_sprite(&m_location)
{
}

GameObject::GameObject(
	game::Float2 location,
	int sprite_layer,
	int sprite_texture,
	game::Float2 sprite_size,
	game::Float2 sprite_offset
)
	:
	m_location(location),
	m_sprite(&m_location, sprite_layer, sprite_texture, sprite_size, sprite_offset)
{
}

GameObject::GameObject(const game::Float2 location, const Sprite& sprite)
	:
	m_location(location),
	m_sprite(&m_location, sprite)
{
}

void GameObject::Update()
{
	// Does nothing
}

bool GameObject::CompareRenderLayer(GameObject* gameObjectA, GameObject* gameObjectB)
{
	return gameObjectA->m_sprite.layer > gameObjectB->m_sprite.layer;
}

bool GameObject::CompareLocationX(GameObject* gameObjectA, GameObject* gameObjectB)
{
	return gameObjectA->m_location.x < gameObjectB->m_location.x;
}

bool GameObject::CompareLocationY(GameObject* gameObjectA, GameObject* gameObjectB)
{
	return gameObjectA->m_location.y > gameObjectB->m_location.y;
}
