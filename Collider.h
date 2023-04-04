#pragma once

#include "DataTypes.h"
#include "WorldObject.h"
#include <vector>

class Collider
{
public:
	WorldObject* pOwner;
	game::Float2& origin;
	bool dynamic;
	bool block;
	bool moving;
	bool trigger;

	std::vector<Collider*> pCollisions;

	game::Float2 moveBuffer;

	Collider(WorldObject* pOwner, bool dynamic = true, bool block = true, bool trigger = false);

	void Update();

	virtual void CheckCollision(Collider* pCollider);
};
class Collider_Box : public Collider
{
public:
	game::Float2 size;

	Collider_Box(WorldObject* pOwner, game::Float2 size = { 1,1 }, bool dynamic = true, bool block = true, bool trigger = false);
	Collider_Box(WorldObject* pOwner, const Collider_Box& collider);

	virtual void CheckCollision(Collider* pCollider) override;
};
class Collider_Circle : public Collider
{
public:
	float radius;

	Collider_Circle(WorldObject* pOwner, float radius = 1.f, bool dynamic = true, bool block = true, bool trigger = false);
	Collider_Circle(WorldObject* pOwner, const Collider_Circle& collider);

	virtual void CheckCollision(Collider* pCollider) override;
};
