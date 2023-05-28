#pragma once

#include "WorldObject.h"
#include "Collider.h"


class Box : public WorldObject
{
protected:
	virtual void WriteData(std::ostream& os) override;

public:
	Collider_Box m_collider;

	virtual void Update(float deltaTime) override;

	Box(
		ImageData* sprite_pImageData,
		int sprite_layer,
		game::float2 sprite_scale,
		game::float2 sprite_offset,
		game::float2 collider_size,
		bool collider_dynamic,
		bool collider_block,
		float sprite_frameTime = 1000.f
	);
	Box(const Box& box);
	Box(const Sprite& sprite, const Collider_Box& collider);
	Box(std::istream& is);

	virtual GameObject* Clone() override;
};
