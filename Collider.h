#pragma once
#include "DataTypes.h"

class Collider
{
public:
	game::Float2& origin;
	bool dynamic;
	bool block;

	game::Float2 moveBuffer;

	Collider(game::Float2* pOrigin, bool dyn = true, bool blck = true);

	void Update();

	virtual void CheckCollision(Collider* pCollider);
};
class Collider_Box : public Collider
{
public:
	game::Float2 size;

	Collider_Box(game::Float2* pOrigin, game::Float2 sz, bool dyn = true, bool blck = true);
	Collider_Box(game::Float2* pOrigin, const Collider_Box& collider);

	virtual void CheckCollision(Collider* pCollider) override;

	Collider_Box& operator =(const Collider_Box& collider)
	{
		origin = collider.origin;
		dynamic = collider.dynamic;
		block = collider.block;
		size = collider.size;
		return *this;
	}
};
class Collider_Circle : public Collider
{
public:
	float radius;

	Collider_Circle(game::Float2* pOrigin, float r, bool dyn = true, bool blck = true);

	virtual void CheckCollision(Collider* pCollider) override;
};
