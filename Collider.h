#pragma once

#include "FileWritable.h"
#include "DataTypes.h"
#include "WorldObject.h"
#include <vector>
#include <string>


class Collider : public FileWritable
{
protected:
	//virtual void WriteData(std::ostream& os) override;

public:
	WorldObject* pOwner;
	bool dynamic;
	bool block;
	bool moving;
	bool trigger;

	std::vector<Collider*> pCollisions;

	game::float2 moveBuffer;


	Collider(WorldObject* pOwner, bool dynamic = true, bool block = true, bool trigger = false);
	Collider(WorldObject* pOwner, std::istream& is);

	void Update();

	virtual void CheckCollision(Collider* pCollider);

	virtual void WriteData(std::ostream& os) override;

};
class Collider_Box : public Collider
{
protected:
	//virtual void WriteData(std::ostream& os) override;

public:
	game::float2 size;

	Collider_Box(WorldObject* pOwner, game::float2 size = { 1,1 }, bool dynamic = true, bool block = true, bool trigger = false);
	Collider_Box(WorldObject* pOwner, const Collider_Box& collider);
	Collider_Box(WorldObject* pOwner, std::istream& is);

	virtual void CheckCollision(Collider* pCollider) override;

	virtual void WriteData(std::ostream& os) override;
};
class Collider_Circle : public Collider
{
protected:
	//virtual void WriteData(std::ostream& os) override;

public:
	float radius;

	Collider_Circle(WorldObject* pOwner, float radius = 1.f, bool dynamic = true, bool block = true, bool trigger = false);
	Collider_Circle(WorldObject* pOwner, const Collider_Circle& collider);
	Collider_Circle(WorldObject* pOwner, std::istream& is);

	virtual void CheckCollision(Collider* pCollider) override;

	virtual void WriteData(std::ostream& os) override;
};
