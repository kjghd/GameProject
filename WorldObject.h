#pragma once
#include "DataTypes.h"
#include "Textures.h"

#include <cmath>
#include <algorithm>
#include <vector>

class Camera
{
public:
	Camera()
		:
		m_zoom(64.f),
		m_speed(6.f),
		m_location({0,0})
	{
	}

	void Move(game::Float2 amount, float deltaTime)
	{
		m_location = m_location + amount * (m_speed * deltaTime / 1000);
	}
	void MoveTo(game::Float2 destination, float deltaTime)
	{
		game::Float2 distance{ destination - m_location };

		m_location += distance * (m_speed * deltaTime / 1000);
	}

	void SetZoom(float amount)
	{
		m_zoom = amount;
	}
	float GetZoom()
	{
		return m_zoom;
	}

	game::Float2 ScreenLocToWorldLoc(float screenX, float screenY)
	{
		// Loc to Corn (SU)
		game::Float2 A{ -screenX, -screenY };
		// WU
		A = {
			SU_to_WU(A.x),
			-SU_to_WU(A.y)
		};

		// Corn to Cam (SU)
		game::Float2 B{
			m_screenResolution.x / 2,
			m_screenResolution.y / 2
		};
		// WU
		B = {
			SU_to_WU(B.x),
			-SU_to_WU(B.y)
		};

		// Cam to Cent (WU)
		game::Float2 C{ -m_location };

		// Loc to Cent
		game::Float2 D{ A+B+C };

		// Cent to Loc
		return -D;
	}
	
	game::Float2 WorldLocToScreenLoc(float worldX, float worldY)
	{
		// Loc to Cent (WU)
		game::Float2 A{ -worldX, -worldY };
		// SU
		A = {
			WU_to_SU(A.x),
			-WU_to_SU(A.y)
		};

		// Cent to Cam (WU)
		game::Float2 B{ m_location };
		// SU
		B = {
			WU_to_SU(B.x),
			-WU_to_SU(B.y)
		};

		// Cam to Corn (SU)
		game::Float2 C{
			-m_screenResolution.x / 2,
			-m_screenResolution.y / 2
		};

		// Loc to Corn
		game::Float2 D{ A+B+C };

		// Corn to Loc
		return -D;
	}

	game::Rect WorldTransformToScreenRect(game::Float2 location, game::Float2 size)
	{
		game::Float2 bottom_left{
			WorldLocToScreenLoc(
				location.x - size.x / 2,
				location.y - size.y / 2
			)
		};
		game::Float2 top_right{
			WorldLocToScreenLoc(
				location.x + size.x / 2,
				location.y + size.y / 2
			)
		};

		return {
			bottom_left.x,
			top_right.y,
			top_right.x,
			bottom_left.y
		};
	}

	static game::Int2 m_screenResolution;

	float SU_to_WU(float screenUnit)
	{
		return screenUnit / m_zoom;
	}
	float WU_to_SU(float worldUnit)
	{
		return worldUnit * m_zoom;
	}

protected:


	game::Float2 m_location;
	float m_zoom;
	float m_speed;

};

class Tile
{
public:
	int m_texture;
	game::Float2 m_location;
	static game::Float2 m_size;
	bool m_block;
	int m_rotation;

	Tile(int texture = T_Error, game::Float2 location = { 0,0 }, bool block = false)
		:
		m_location(location),
		m_texture(texture),
		m_block(block)
	{
	}
};

class Character
{
public:
	game::Float2 m_location;

	int m_texture;
	game::Float2 m_spriteOffset;
	game::Float2 m_size;

	game::Float2 m_collisionSize;
	float m_collisionRadius;

	float m_speed;
	float m_health;

	Character()
		:
		m_location({ 0,0 }),
		m_spriteOffset({ 0,.5 }),
		m_size({ 1,1.5 }),
		m_collisionSize({ .8,.8 }),
		m_collisionRadius(.4),
		m_texture(T_Shadow),
		m_speed(5)
	{

	}

	void Update(float deltaTime)
	{

	}

	void Move(game::Float2 direction, float deltaTime)
	{
		m_location += direction * (m_speed * deltaTime / 1000.f);
	}

	void Collide_BoxVsBox(Tile& tile)
	{
		// Object A (this)
		float at = m_location.y + m_collisionSize.y / 2;
		float ab = m_location.y - m_collisionSize.y / 2;
		float al = m_location.x - m_collisionSize.x / 2;
		float ar = m_location.x + m_collisionSize.x / 2;
		// Object B (target)
		float bt = tile.m_location.y + tile.m_size.y / 2;
		float bb = tile.m_location.y - tile.m_size.y / 2;
		float bl = tile.m_location.x - tile.m_size.x / 2;
		float br = tile.m_location.x + tile.m_size.x / 2;

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
			if (tile.m_block)
			{
				if (t == shortest) m_location.y -= t;
				if (b == shortest) m_location.y += b;
				if (l == shortest) m_location.x += l;
				if (r == shortest) m_location.x -= r;
			}
		}
	}
	void Collide_CircleVsBox(Tile& tile)
	{
		game::Float2 point{
				std::clamp<float>(
					m_location.x,
					tile.m_location.x - tile.m_size.x / 2,
					tile.m_location.x + tile.m_size.x / 2
				),
				std::clamp<float>(
					m_location.y,
					tile.m_location.y - tile.m_size.y / 2,
					tile.m_location.y + tile.m_size.y / 2
				)
		};

		game::Float2 vec{ point - m_location };

		float length = std::sqrtf(std::powf(vec.x, 2.f) + std::powf(vec.y, 2.f));
		float overlap = m_collisionRadius - length;

		if (overlap >= 0)
		{
			if (tile.m_block)
			{
				m_location -= vec * (overlap / length);
			}
		}
	}
};

class Player
{
public:
	game::Float2 m_location;
	game::Float2 m_spriteOffset;
	game::Float2 m_size;
	game::Float2 m_collisionSize;
	float m_collisionRadius;
	int m_texture;
	float m_speed;
	float m_health;

	Player()
		:
		m_location({ 0,0 }),
		m_spriteOffset({ 0,.5 }),
		m_size({ 1,1.5 }),
		m_collisionSize({ .8,.8 }),
		m_collisionRadius(.4),
		m_texture(T_Shadow),
		m_speed(5)
	{

	}

	void Update(float deltaTime)
	{

	}

	void Move(game::Float2 direction, float deltaTime)
	{
		m_location += direction * (m_speed * deltaTime / 1000.f);
	}

	void Collide_BoxVsBox(Tile& tile)
	{
		// Object A (this)
		float at = m_location.y + m_collisionSize.y / 2;
		float ab = m_location.y - m_collisionSize.y / 2;
		float al = m_location.x - m_collisionSize.x / 2;
		float ar = m_location.x + m_collisionSize.x / 2;
		// Object B (target)
		float bt = tile.m_location.y + tile.m_size.y / 2;
		float bb = tile.m_location.y - tile.m_size.y / 2;
		float bl = tile.m_location.x - tile.m_size.x / 2;
		float br = tile.m_location.x + tile.m_size.x / 2;

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
			if (tile.m_block)
			{
				if (t == shortest) m_location.y -= t;
				if (b == shortest) m_location.y += b;
				if (l == shortest) m_location.x += l;
				if (r == shortest) m_location.x -= r;
			}
		}
	}
	void Collide_CircleVsBox(Tile& tile)
	{
		game::Float2 point{
				std::clamp<float>(
					m_location.x,
					tile.m_location.x - tile.m_size.x / 2,
					tile.m_location.x + tile.m_size.x / 2
				),
				std::clamp<float>(
					m_location.y,
					tile.m_location.y - tile.m_size.y / 2,
					tile.m_location.y + tile.m_size.y / 2
				)
		};

		game::Float2 vec{ point - m_location };

		float length = std::sqrtf(std::powf(vec.x, 2.f) + std::powf(vec.y, 2.f));
		float overlap = m_collisionRadius - length;

		if (overlap >= 0)
		{
			if (tile.m_block)
			{
				m_location -= vec * (overlap / length);
			}
		}
	}
};

