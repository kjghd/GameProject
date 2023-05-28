#pragma once
#include "FileWritable.h"
#include "DataTypes.h"
#include "Sprite.h"
#include <ostream>


class GameObject : public FileWritable
{
protected:
	virtual void WriteData(std::ostream& os) override;

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
	GameObject(std::istream& is);

	virtual void Update(float deltaTime);
	virtual void Update_SpriteOnly();

	virtual GameObject* Clone();

};
