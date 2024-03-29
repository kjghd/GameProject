#include "Input.h"

struct Button
{
	bool on = false;
	bool moved = false;
};

Button Input::buttons[BTN_COUNT];
D2D1_POINT_2F Input::mouseLoc{ 0,0 };
float Input::scrollDistance;

Input::Input()
{
}

void Input::ResetMoved()
{
	for (size_t i{ 0 }; i < BTN_COUNT; ++i)
		buttons[i].moved = false;

	scrollDistance = 0;
}

void Input::SetMouseLoc(D2D1_POINT_2F location)
{
	mouseLoc = location;
}
D2D1_POINT_2F Input::GetMouseLoc()
{
	return mouseLoc;
}
void Input::SetScrollDistance(float distance)
{
	scrollDistance = distance;
}
float Input::GetScrollDistance()
{
	return scrollDistance / WHEEL_DELTA;
}

void Input::SetOn(size_t button)
{
	if (!buttons[button].on)
	{
		buttons[button].on = true;
		buttons[button].moved = true;
	}
}
void Input::SetOff(size_t button)
{
	if (buttons[button].on)
	{
		buttons[button].on = false;
		buttons[button].moved = true;
	}
}

bool Input::CheckPressed(size_t button)
{
	if (buttons[button].on && buttons[button].moved)
		return true;
	else
		return false;
}
bool Input::CheckHeld(size_t button)
{
	if (buttons[button].on)
		return true;
	else
		return false;
}
bool Input::CheckReleased(size_t button)
{
	if (!buttons[button].on && buttons[button].moved)
		return true;
	else
		return false;
}