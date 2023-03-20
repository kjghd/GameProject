#include "NPC.h"
#include <random>

NPC::NPC(float damage,
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
	float sprite_frameTime
	)
	:
	Character(
		health,
		speed,
		sprite_pImageData,
		sprite_layer,
		sprite_scale,
		sprite_offset,
		collider_radius,
		collider_dynamic,
		collider_block,
		view_range,
		sprite_frameTime
	),
	m_damage(damage),
	wanderDirection{0,0},
	wanderCooldown(0),
	wanderTime(0)
{
}

NPC::NPC(const NPC& npc)
	:
	Character(npc.m_sprite, npc.m_collider, npc.m_viewRange, npc.m_health, npc.m_speed),
	m_damage(npc.m_damage),
	wanderDirection{0,0},
	wanderCooldown(0),
	wanderTime(0)
{
}

void NPC::Update(float deltaTime)
{
	if (m_health > 0)
	{
		bool targeting{ false };
		for (auto& pViewTarget : m_viewRange.pCollisions)
		{
			if (dynamic_cast<Character*>(pViewTarget->pOwner))
			{
				Character* pCharacter{ dynamic_cast<Character*>(pViewTarget->pOwner) };
				if (!dynamic_cast<NPC*>(pViewTarget->pOwner) && pCharacter->m_health > 0)
				{
					Move(pCharacter->m_location - m_location);
					targeting = true;
					wanderTime = 0;
				}
			}
		}
		for (auto& pCollision : m_collider.pCollisions)
			if (dynamic_cast<Character*>(pCollision->pOwner))
			{
				Character* pCharacter{ dynamic_cast<Character*>(pCollision->pOwner) };
				if (!dynamic_cast<NPC*>(pCollision->pOwner))
				{
					pCharacter->m_health -= m_damage;
				}
			}


		if (!targeting)
		{
			Wander(deltaTime);
		}

		ApplyMovement(deltaTime);
	}
	else if (m_sprite.GetCurrentAnimation() != 2)
		m_sprite.SetAnimation(2);

	m_sprite.Update(deltaTime);
	m_collider.Update();
	m_viewRange.Update();
}


void NPC::Wander(float deltaTime)
{
	if (wanderTime <= 0)
	{
		wanderCooldown -= deltaTime;
		if (wanderCooldown <= 0)
		{
			srand(static_cast<unsigned int>(deltaTime * 1000));

			float x = -1 + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / 2));
			float y = -1 + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / 2));

			wanderDirection = { x, y };

			wanderTime = 10000 * (x >= 0 ? x : -x);
			wanderCooldown = 10000 * (y >= 0 ? y : -y);
		}
	}
	else
	{
		Move(wanderDirection);
		wanderTime -= deltaTime;
	}

}
