#include "GameObject.h"


GameObject::GameObject()
	:
	m_location{ 0,0 },
	m_sprite(Sprite(&m_location, nullptr, 0))
{
}

GameObject::GameObject(
	ImageData* sprite_pImageData,
	float sprite_frameTime,
	int sprite_layer,
	game::Float2 sprite_scale,
	game::Float2 sprite_offset
)
	:
	m_location{0,0},
	m_sprite(Sprite(&m_location, sprite_pImageData, sprite_frameTime, sprite_layer, sprite_scale, sprite_offset))
{
}

GameObject::GameObject(const Sprite& sprite)
	:
	m_location{0,0},
	m_sprite(Sprite(& m_location, sprite))
{
}

void GameObject::Update(float deltaTime)
{
	m_sprite.Update(deltaTime);
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
