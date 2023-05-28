#pragma once
#include "Character.h"

class NPC : public Character
{
	game::float2 wanderDirection;
	float wanderCooldown;
	float wanderTime;

protected:
	virtual void WriteData(std::ostream& os) override;

public:

	float m_damage;

	NPC(float damage,
		float health,
		float speed,
		ImageData* sprite_pImageData,
		int sprite_layer,
		game::float2 sprite_scale,
		game::float2 sprite_offset,
		float collider_radius,
		bool collider_dynamic,
		bool collider_block,
		float view_range,
		float sprite_frameTime = 1000.f
	);
	NPC(const NPC& npc);
	NPC(std::istream& is);

	virtual void Update(float deltaTime) override;

	void Wander(float deltaTime);

	virtual GameObject* Clone() override;
};
