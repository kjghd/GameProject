#include "Timer.h"

void Timer::Start()
{
	QueryPerformanceFrequency(&frequency);
	QueryPerformanceCounter(&startTime);
}

void Timer::Tick()
{
	QueryPerformanceCounter(&endTime);
	deltaTime = ((double)endTime.QuadPart - (double)startTime.QuadPart) / (double)frequency.QuadPart * 1000.0;
	QueryPerformanceCounter(&startTime);
}