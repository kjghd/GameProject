#pragma once
#include "Character.h"

class NPC : public Character
{
	game::Float2 wanderDirection;
	float wanderCooldown;
	float wanderTime;

public:

	float m_damage;

	NPC(float damage,
		float health,
		float speed,
		ImageData* sprite_pImageData,
		int sprite_layer,
		game::Float2 sprite_scale,
		game::Float2 sprite_offset,
		float collider_radius,
		bool collider_dynamic,
		bool collider_block,
		float view_range,
		float sprite_frameTime = 1000.f
	);

	NPC(const NPC& npc);

	virtual void Update(float deltaTime) override;

	void Wander(float deltaTime);

	NPC& operator =(const NPC& npc)
	{
		m_location = npc.m_location;
		m_sprite = Sprite(&m_location, npc.m_sprite);
		m_collider = Collider_Circle(this, npc.m_collider);
		m_speed = npc.m_speed;
		m_viewRange = Collider_Circle(this, npc.m_viewRange);

		return *this;
	}
};

