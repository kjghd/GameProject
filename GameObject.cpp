#include "GameObject.h"

// Used to create prefabs.
GameObject::GameObject(
	ImageData* sprite_pImageData,
	int sprite_layer,
	game::float2 sprite_scale,
	game::float2 sprite_offset,
	float sprite_frameTime
)
	:
	m_location{ 0,0 },
	m_sprite(this, sprite_pImageData, sprite_frameTime, sprite_layer, sprite_scale, sprite_offset)
{
}

// Used when spawing from a prefab in a scene.
GameObject::GameObject(const GameObject& gameObject)
	:
	m_location(gameObject.m_location),
	m_sprite(this, gameObject.m_sprite)
{
}

// Used by derived classes.
GameObject::GameObject(const Sprite& sprite)
	:
	m_location{ 0,0 },
	m_sprite(this, sprite)
{
}

//GameObject::GameObject(std::ifstream& file)
//	:
//	m_location{(float)file.get(), (float)file.get()}
//{
//
//}

void GameObject::Update(float deltaTime)
{
	m_sprite.Update(deltaTime);
}

void GameObject::Update_SpriteOnly()
{
	m_sprite.Pause();
	m_sprite.Update(0);
}

std::string GameObject::Serialise()
{
	std::string str;
	
	//str += game::DataToString<game::float2>(m_location);
	//str += m_sprite.Serialise();
	
	return str;
}
