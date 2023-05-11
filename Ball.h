#pragma once
#include "DataTypes.h"
#include "WorldObject.h"
#include "Collider.h"
#include <memory>


class Ball : public WorldObject
{
public:
	Collider_Circle m_collider;
	
	virtual void Update(float deltaTime) override;

	Ball(
		ImageData* sprite_pImageData,
		int sprite_layer,
		game::float2 sprite_scale,
		game::float2 sprite_offset,
		float collider_radius,
		bool collider_dynamic,
		bool collider_block,
		float sprite_frameTime = 1000.f
	);
	Ball(const Ball& ball);
	Ball(const Sprite& sprite, const Collider_Circle& collider);

	virtual std::string Serialise() override;
};
