#include "Collider.h"

#include <cmath>
#include <algorithm>

// Collider
Collider::Collider(game::Float2* pOrigin, bool dyn, bool blck)
	:
	origin(*pOrigin),
	dynamic(dyn),
	block(blck),
	moveBuffer{ 0,0 }
{
}

void Collider::CheckCollision(Collider* pCollider)
{
	// Empty
}

void Collider::Update()
{
	origin += moveBuffer;
	moveBuffer = { 0,0 };
}

// Collider Box
Collider_Box::Collider_Box(game::Float2* pOrigin, game::Float2 sz, bool dyn, bool blck)
	:
	Collider(pOrigin, dyn, blck),
	size(sz)
{
}
Collider_Box::Collider_Box(game::Float2* pOrigin, const Collider_Box& collider)
	:
	Collider(pOrigin, collider.dynamic, collider.block),
	size(collider.size)
{
}

void Collider_Box::CheckCollision(Collider* pCollider)
{
	// Box vs circle
	if (dynamic_cast<Collider_Circle*>(pCollider))
	{
		Collider_Circle* circle{ dynamic_cast<Collider_Circle*>(pCollider) };

		// Closest part of box to cicle.
		game::Float2 point{
		std::clamp<float>(
			circle->origin.x,
			origin.x - size.x / 2,
			origin.x + size.x / 2
		),
		std::clamp<float>(
			circle->origin.y,
			origin.y - size.y / 2,
			origin.y + size.y / 2
		),
		};

		// Vector from box point to circle center.
		game::Float2 vec{ circle->origin - point };

		float length = std::sqrtf(std::powf(vec.x, 2.f) + std::powf(vec.y, 2.f));
		float overlap = circle->radius - length;

		if (overlap >= 0)
		{
			if (block && circle->block && dynamic)
			{
				moveBuffer -= vec * (overlap / length);
			}
		}
	}
	// Box vs box
	else if (dynamic_cast<Collider_Box*>(pCollider))
	{
		Collider_Box* box{ dynamic_cast<Collider_Box*>(pCollider) };

		// Object A (this)
		float at = origin.y + size.y / 2;
		float ab = origin.y - size.y / 2;
		float al = origin.x - size.x / 2;
		float ar = origin.x + size.x / 2;
		// Object B (target)
		float bt = box->origin.y + box->size.y / 2;
		float bb = box->origin.y - box->size.y / 2;
		float bl = box->origin.x - box->size.x / 2;
		float br = box->origin.x + box->size.x / 2;

		// Overlap
		float t = at - bb;
		float b = bt - ab;
		float l = br - al;
		float r = ar - bl;
		// Shortest overlap
		float shortest_y = t < b ? t : b;
		float shortest_x = l < r ? l : r;
		float shortest = shortest_x < shortest_y ? shortest_x : shortest_y;

		if (shortest >= 0)
		{
			if (block && box->block && dynamic)
			{
				if (t == shortest) moveBuffer.y -= t;
				if (b == shortest) moveBuffer.y += b;
				if (l == shortest) moveBuffer.x += l;
				if (r == shortest) moveBuffer.x -= r;
			}
		}
	}
};


// Collider Circle
Collider_Circle::Collider_Circle(game::Float2* pOrigin, float r, bool dyn, bool blck)
	:
	Collider(pOrigin, dyn, blck),
	radius(r)
{
}

void Collider_Circle::CheckCollision(Collider* pCollider)
{
	// Circle vs circle
	if (dynamic_cast<Collider_Circle*>(pCollider))
	{
		Collider_Circle* circle{ dynamic_cast<Collider_Circle*>(pCollider) };

	}
	// Circle vs box
	else if (dynamic_cast<Collider_Box*>(pCollider))
	{
		Collider_Box* box{ dynamic_cast<Collider_Box*>(pCollider) };

		game::Float2 point{
		std::clamp<float>(
			origin.x,
			box->origin.x - box->size.x / 2,
			box->origin.x + box->size.x / 2
		),
		std::clamp<float>(
			origin.y,
			box->origin.y - box->size.y / 2,
			box->origin.y + box->size.y / 2
		)
		};

		game::Float2 vec{ point - origin };

		float length = std::sqrtf(std::powf(vec.x, 2.f) + std::powf(vec.y, 2.f));
		float overlap = radius - length;

		if (overlap >= 0)
		{
			if (block && box->block && dynamic)
			{
				moveBuffer -= vec * (overlap / length);
			}
		}
	}
};
