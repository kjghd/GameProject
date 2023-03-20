#pragma once

#include <Windows.h>


class Timer
{
	LARGE_INTEGER frequency;
	LARGE_INTEGER startTime;
	LARGE_INTEGER endTime;

public:
	double deltaTime;

	void Start();
	void Tick();
};

