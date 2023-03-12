#pragma once

#include "Ball.h"


class Player : public Ball
{
public:
	float m_speed;

	Player();

	virtual void Update(float deltaTime) override;

	void Move(game::Float2 direction, float deltaTime);
};
