#pragma once

#include "Ball.h"


class Player : public Ball
{
public:
	float m_speed;

	Player();

	void Update(float deltaTime);

	void Move(game::Float2 direction, float deltaTime);
};
