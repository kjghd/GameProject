#pragma once
#include "Character.h"
#include "Camera.h"


class Player : public Character
{
public:
    Camera m_camera;

	Player(
		float health,
		float speed,
		ImageData* sprite_pImageData,
		int sprite_layer,
		game::float2 sprite_scale,
		game::float2 sprite_offset,
		float collider_radius,
		bool collider_dynamic,
		bool collider_block,
		float view_radius,
		float sprite_frameTime = 1000.f
	);

	Player(const Player& player);

    virtual void Update(float deltaTime) override;

	virtual std::string Serialise() override;
};
