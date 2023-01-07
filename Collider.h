#pragma once

#include "DataTypes.h"


class Collider
{
public:
	game::Float2& origin;
	bool dynamic;
	bool block;

	game::Float2 moveBuffer;

	Collider(game::Float2* pOrigin, bool dynamic = true, bool block = true);

	void Update();

	virtual void CheckCollision(Collider* pCollider);
};
class Collider_Box : public Collider
{
public:
	game::Float2 size;

	Collider_Box(game::Float2* pOrigin, game::Float2 size = { 1,1 }, bool dynamic = true, bool block = true);
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

	Collider_Circle(game::Float2* pOrigin, float radius = 1.f, bool dynamic = true, bool block = true);
	Collider_Circle(game::Float2* pOrigin, const Collider_Circle& collider);

	virtual void CheckCollision(Collider* pCollider) override;

	Collider_Circle& operator =(const Collider_Circle& collider)
	{
		origin = collider.origin;
		dynamic = collider.dynamic;
		block = collider.block;
		radius = collider.radius;
		return *this;
	}
};
