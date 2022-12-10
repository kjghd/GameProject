#pragma once

#include <D2D1.h>


enum Buttons
{
	BTN_0,
	BTN_1,
	BTN_2,
	BTN_3,
	BTN_4,
	BTN_5,
	BTN_6,
	BTN_7,
	BTN_8,
	BTN_9,

	BTN_A,
	BTN_D,
	BTN_E,
	BTN_Q,
	BTN_S,
	BTN_W,

	BTN_LMB,
	BTN_RMB,

	BTN_COUNT,
};

class Input
{
	struct Button
	{
		bool on = false;
		bool moved = false;
	};

	Button buttons[BTN_COUNT];

	D2D1_POINT_2F mouseLoc;

public:
	Input();

	void ResetMoved();

	void SetMouseLoc(D2D1_POINT_2F location);
	D2D1_POINT_2F GetMouseLoc();

	void SetOn(size_t button);
	void SetOff(size_t button);

	bool CheckPressed(size_t button);
	bool CheckHeld(size_t button);
	bool CheckReleased(size_t button);
};

