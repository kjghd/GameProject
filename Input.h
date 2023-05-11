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

	BTN_SHIFT,

	BTN_TAB,
	BTN_ESC,

	BTN_COUNT,
};

struct Button;

class Input
{
	static Button buttons[BTN_COUNT];
	static D2D1_POINT_2F mouseLoc;
	static float scrollDistance;

	Input();

public:
	static void ResetMoved();

	static void SetMouseLoc(D2D1_POINT_2F location);
	static D2D1_POINT_2F GetMouseLoc();
	static void SetScrollDistance(float distance);
	static float GetScrollDistance();

	static void SetOn(size_t button);
	static void SetOff(size_t button);

	static bool CheckPressed(size_t button);
	static bool CheckHeld(size_t button);
	static bool CheckReleased(size_t button);
};

