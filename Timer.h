#pragma once
#include <Windows.h>


class Timer
{
	LARGE_INTEGER m_frequency;
	LARGE_INTEGER m_startTime;
	LARGE_INTEGER m_endTime;
	double m_deltaTime;

public:
	void Start();
	void Tick();
	double GetDeltaTime();
};
