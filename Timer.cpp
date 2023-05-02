#include "Timer.h"


void Timer::Start()
{
	QueryPerformanceFrequency(&m_frequency);
	QueryPerformanceCounter(&m_startTime);
}

void Timer::Tick()
{
	QueryPerformanceCounter(&m_endTime);
	m_deltaTime = ((double)m_endTime.QuadPart - (double)m_startTime.QuadPart) / (double)m_frequency.QuadPart * 1000.0;
	QueryPerformanceCounter(&m_startTime);
}

double Timer::GetDeltaTime()
{
	return m_deltaTime;
}
