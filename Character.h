#pragma once

#include "Ball.h"


class Character : public Ball
{
public:
	float m_speed;

	Character();

	virtual void Update(float deltaTime) override;

	void Move(game::Float2 direction, float deltaTime);
};
