#include "Collider.h"

#include <cmath>
#include <algorithm>


// Functions
struct CollisionInfo
{
	game::float2 vec;
	float overlap;
	float length;
};

CollisionInfo BoxVsBox(Collider_Box* pA, Collider_Box* pB)
{
	float at = pA->pOwner->m_location.y + pA->size.y / 2;
	float ab = pA->pOwner->m_location.y - pA->size.y / 2;
	float al = pA->pOwner->m_location.x - pA->size.x / 2;
	float ar = pA->pOwner->m_location.x + pA->size.x / 2;
	float bt = pB->pOwner->m_location.y + pB->size.y / 2;
	float bb = pB->pOwner->m_location.y - pB->size.y / 2;
	float bl = pB->pOwner->m_location.x - pB->size.x / 2;
	float br = pB->pOwner->m_location.x + pB->size.x / 2;

	// Overlap
	float t = at - bb;
	float b = bt - ab;
	float l = br - al;
	float r = ar - bl;

	// Shortest overlap
	float shortest_y = t < b ? t : b;
	float shortest_x = l < r ? l : r;
	float shortest = shortest_x < shortest_y ? shortest_x : shortest_y;

	game::float2 vec;
	if (t == shortest) vec = {  0,  1 };
	if (b == shortest) vec = {  0, -1 };
	if (l == shortest) vec = { -1,  0 };
	if (r == shortest) vec = {  1,  0 };

	return CollisionInfo{vec, shortest, 1};
}
CollisionInfo CircleVsCircle(Collider_Circle* pA, Collider_Circle* pB)
{
	float total_radii = pA->radius + pB->radius;
	game::float2 vec{ pB->pOwner->m_location - pA->pOwner->m_location };
	float distance = std::sqrtf(std::powf(vec.x, 2.f) + std::powf(vec.y, 2.f));
	float overlap = total_radii - distance;

	return CollisionInfo{ vec, overlap, distance };
}
CollisionInfo CircleVsBox(Collider_Circle* pCircle, Collider_Box* pBox, bool invert)
{
	game::float2 clamped_point{
		std::clamp<float>(
			pCircle->pOwner->m_location.x,
			pBox->pOwner->m_location.x - pBox->size.x / 2,
			pBox->pOwner->m_location.x + pBox->size.x / 2
		),
		std::clamp<float>(
			pCircle->pOwner->m_location.y,
			pBox->pOwner->m_location.y - pBox->size.y / 2,
			pBox->pOwner->m_location.y + pBox->size.y / 2
		),
	};

	game::float2 vec{ clamped_point - pCircle->pOwner->m_location };
	float distance = std::sqrtf(std::powf(vec.x, 2.f) + std::powf(vec.y, 2.f));
	float overlap = pCircle->radius - distance;

	return CollisionInfo{ invert ? -vec : vec, overlap, distance };
}

void DetermineCollision(Collider* pA, Collider* pB, CollisionInfo col_info)
{
	if (col_info.overlap >= 0)
	{
		if (pA->trigger) pA->pCollisions.push_back(pB);
		if (pA->block && pB->block && pA->dynamic)
		{
			if ((pA->moving && pB->moving) || (!pA->moving && !pB->moving) || (!pB->dynamic))
			{
				pA->moveBuffer -= col_info.vec * (col_info.overlap / col_info.length);
			}
			else if (!pA->moving && pB->moving)
			{
				pA->moveBuffer -= col_info.vec * (col_info.overlap / col_info.length * 2);
			}
		}
	}
}


// Collider
Collider::Collider(WorldObject* pOwner, bool dynamic, bool block, bool trigger)
	:
	pOwner(pOwner),
	dynamic(dynamic),
	block(block),
	moving(false),
	trigger(trigger),
	moveBuffer{ 0,0 }
{
	m_tag = "COLL";
}
Collider::Collider(WorldObject* pOwner, std::istream& is)
	:
	pOwner(pOwner),
	dynamic(FileWritable::GetNextValue(is) == "1" ? true : false),
	block(FileWritable::GetNextValue(is) == "1" ? true : false),
	moving(FileWritable::GetNextValue(is) == "1" ? true : false),
	trigger(FileWritable::GetNextValue(is) == "1" ? true : false),
	moveBuffer{ std::stof(FileWritable::GetNextValue(is)),std::stof(FileWritable::GetNextValue(is)) }
{
	m_tag = "COLL";
}

void Collider::CheckCollision(Collider* pCollider)
{
	// Empty
}

void Collider::Update()
{
	moving = false;
	pOwner->m_location += moveBuffer;
	moveBuffer = { 0,0 };
	pCollisions.clear();
}

void Collider::WriteData(std::ostream& os)
{
	os << dynamic		<< ',';
	os << block			<< ',';
	os << moving		<< ',';
	os << trigger		<< ',';
	os << moveBuffer.x	<< ',';
	os << moveBuffer.y;
}


// Collider Box
Collider_Box::Collider_Box(WorldObject* pOwner, game::float2 sz, bool dyn, bool blck, bool trigger)
	:
	Collider(pOwner, dyn, blck, trigger),
	size(sz)
{
	m_tag = "COLB";
}
Collider_Box::Collider_Box(WorldObject* pOwner, const Collider_Box& collider)
	:
	Collider(pOwner, collider.dynamic, collider.block, collider.trigger),
	size(collider.size)
{
	m_tag = "COLB";
}
Collider_Box::Collider_Box(WorldObject* pOwner, std::istream& is)
	:
	Collider(pOwner, is),
	size{ std::stof(FileWritable::GetNextValue(is)), std::stof(FileWritable::GetNextValue(is)) }
{
	m_tag = "COLB";
}

void Collider_Box::CheckCollision(Collider* pCollider)
{
	// Box vs circle
	if (dynamic_cast<Collider_Circle*>(pCollider))
	{
		Collider_Circle* pCircle{ dynamic_cast<Collider_Circle*>(pCollider) };
		CollisionInfo collisionInfo{ CircleVsBox(pCircle, this, true) };

		DetermineCollision(this, pCircle, collisionInfo);
	}
	// Box vs box
	else if (dynamic_cast<Collider_Box*>(pCollider))
	{
		Collider_Box* pBox{ dynamic_cast<Collider_Box*>(pCollider) };
		CollisionInfo collision_info{ BoxVsBox(this, pBox) };

		DetermineCollision(this, pBox, collision_info);
	}
};

void Collider_Box::WriteData(std::ostream& os)
{
	Collider::WriteData(os);
	os << ',';
	os << size.x << ',';
	os << size.y;
}


// Collider Circle
Collider_Circle::Collider_Circle(WorldObject* pOwner, float radius, bool dynamic, bool block, bool trigger)
	:
	Collider(pOwner, dynamic, block, trigger),
	radius(radius)
{
	m_tag = "COLC";
}
Collider_Circle::Collider_Circle(WorldObject* pOwner, const Collider_Circle& collider)
	:
	Collider(pOwner, collider.dynamic, collider.block, collider.trigger),
	radius(collider.radius)
{
	m_tag = "COLC";
}
Collider_Circle::Collider_Circle(WorldObject* pOwner, std::istream& is)
	:
	Collider(pOwner, is),
	radius(std::stof(FileWritable::GetNextValue(is)))
{
}
void Collider_Circle::CheckCollision(Collider* pCollider)
{
	// Circle vs circle
	if (dynamic_cast<Collider_Circle*>(pCollider))
	{
		Collider_Circle* pCircle{ dynamic_cast<Collider_Circle*>(pCollider) };
		CollisionInfo collisionInfo{ CircleVsCircle(this, pCircle) };

		DetermineCollision(this, pCircle, collisionInfo);
	}
	// Circle vs box
	else if (dynamic_cast<Collider_Box*>(pCollider))
	{
		Collider_Box* pBox{ dynamic_cast<Collider_Box*>(pCollider) };
		CollisionInfo collisionInfo{ CircleVsBox(this, pBox, false) };

		DetermineCollision(this, pBox, collisionInfo);
	}
};

void Collider_Circle::WriteData(std::ostream& os)
{
	Collider::WriteData(os);
	os << ',';
	os << radius;
}
