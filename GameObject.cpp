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
	m_tag = "GOBJ";
}

// Used when spawing from a prefab in a scene.
GameObject::GameObject(const GameObject& gameObject)
	:
	m_location(gameObject.m_location),
	m_sprite(this, gameObject.m_sprite)
{
	m_tag = "GOBJ";
}

// Used by derived classes.
GameObject::GameObject(const Sprite& sprite)
	:
	m_location{ 0,0 },
	m_sprite(this, sprite)
{
	m_tag = "GOBJ";
}

// Used with file data.
GameObject::GameObject(std::istream& is)
	:
	m_location{ std::stof(FileWritable::GetNextValue(is)), std::stof(FileWritable::GetNextValue(is)) },
	m_sprite(this, is)
{
	m_tag = "GOBJ";
}

void GameObject::Update(float deltaTime)
{
	m_sprite.Update(deltaTime);
}

void GameObject::Update_SpriteOnly()
{
	m_sprite.Pause();
	m_sprite.Update(0);
}

GameObject* GameObject::Clone()
{
	return new GameObject(*this);
}

void GameObject::WriteData(std::ostream& os)
{
	os << m_location.x << ',';
	os << m_location.y << ',';
	m_sprite.WriteData(os);
}
