#include "Graphics.h"
#include "Renderer.h"
#include "Timer.h"
#include "Input.h"
#include "SceneManager.h"
#include "Camera.h"
#include "ScreenObject.h"
#include "ImageDataList.h"
#include "Prefabs.h"

#include <Windows.h>
#include <Windowsx.h>


// Globals.
static Graphics g_graphics;
static Renderer g_renderer;
static Timer g_timer;
static SceneManager g_SceneController;

// Window Procedure.
LRESULT WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	// Close window.
	if (uMsg == WM_CLOSE)
	{
		PostQuitMessage(0);
	}

	// Input
	// Mouse
	else if (uMsg == WM_MOUSEMOVE)
	{
		Input::SetMouseLoc(
			D2D1::Point2F(
				static_cast<FLOAT>(GET_X_LPARAM(lParam)),
				static_cast<FLOAT>(GET_Y_LPARAM(lParam))
			)
		);
	}
	else if (uMsg == WM_LBUTTONDOWN) Input::SetOn(BTN_LMB);
	else if (uMsg == WM_RBUTTONDOWN) Input::SetOn(BTN_RMB);
	else if (uMsg == WM_LBUTTONUP) Input::SetOff(BTN_LMB);
	else if (uMsg == WM_RBUTTONUP) Input::SetOff(BTN_RMB);
	else if (uMsg == WM_MOUSEWHEEL) Input::SetScrollDistance(GET_WHEEL_DELTA_WPARAM(wParam));

	// Keyboard
	else if (uMsg == WM_KEYDOWN)
	{
		switch (wParam)
		{
		case 0x30: Input::SetOn(BTN_0); break;
		case 0x31: Input::SetOn(BTN_1); break;
		case 0x32: Input::SetOn(BTN_2); break;
		case 0x33: Input::SetOn(BTN_3); break;
		case 0x34: Input::SetOn(BTN_4); break;
		case 0x35: Input::SetOn(BTN_5); break;
		case 0x36: Input::SetOn(BTN_6); break;
		case 0x37: Input::SetOn(BTN_7); break;
		case 0x38: Input::SetOn(BTN_8); break;
		case 0x39: Input::SetOn(BTN_9); break;
		case 0x41: Input::SetOn(BTN_A); break;
		case 0x44: Input::SetOn(BTN_D); break;
		case 0x45: Input::SetOn(BTN_E); break;
		case 0x51: Input::SetOn(BTN_Q); break;
		case 0x53: Input::SetOn(BTN_S); break;
		case 0x57: Input::SetOn(BTN_W); break;

		case VK_SHIFT: Input::SetOn(BTN_SHIFT); break;
		case VK_TAB: Input::SetOn(BTN_TAB); break;
		case VK_ESCAPE: Input::SetOn(BTN_ESC); break;
		}
	}
	else if (uMsg == WM_KEYUP)
	{
		switch (wParam)
		{
		case 0x30: Input::SetOff(BTN_0); break;
		case 0x31: Input::SetOff(BTN_1); break;
		case 0x32: Input::SetOff(BTN_2); break;
		case 0x33: Input::SetOff(BTN_3); break;
		case 0x34: Input::SetOff(BTN_4); break;
		case 0x35: Input::SetOff(BTN_5); break;
		case 0x36: Input::SetOff(BTN_6); break;
		case 0x37: Input::SetOff(BTN_7); break;
		case 0x38: Input::SetOff(BTN_8); break;
		case 0x39: Input::SetOff(BTN_9); break;
		case 0x41: Input::SetOff(BTN_A); break;
		case 0x44: Input::SetOff(BTN_D); break;
		case 0x45: Input::SetOff(BTN_E); break;
		case 0x51: Input::SetOff(BTN_Q); break;
		case 0x53: Input::SetOff(BTN_S); break;
		case 0x57: Input::SetOff(BTN_W); break;

		case VK_SHIFT: Input::SetOff(BTN_SHIFT); break;
		case VK_TAB: Input::SetOff(BTN_TAB); break;
		case VK_ESCAPE: Input::SetOff(BTN_ESC); break;
		}
	}

	// Default window procedure
	else return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

// Main Function.
int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow)
{
	// Window Class
	WNDCLASS wc{};
	wc.hInstance = hInstance;
	wc.lpfnWndProc = WndProc;
	wc.lpszClassName = L"Class";
	RegisterClass(&wc);

	// Remove cursor.
	ShowCursor(FALSE);

	// Window Dimensions
	float width  = 1280.f;
	float height = 720.f;
	RECT wndRect{ 0,0, width, height };
	AdjustWindowRect(&wndRect, WS_CAPTION, FALSE);

	// Create Window
	HWND hWnd = CreateWindow(
		L"Class",
		L"Window",
		WS_SYSMENU,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		wndRect.right - wndRect.left,
		wndRect.bottom - wndRect.top,
		nullptr,
		nullptr,
		hInstance,
		nullptr
	);
	ShowWindow(hWnd, nCmdShow);


	ImageDataList::Initialise();
	PrefabList::Initialise();
	g_SceneController.Initialise();

	Camera::m_screenResolution = { width,height };
	ScreenObject::screenRes = { width, height };
	ScreenObject::px_per_su = { 64.f };

	// Start timer
	g_graphics.Init(hWnd);
	g_renderer.Init(&g_graphics, &g_SceneController);
	g_timer.Start();


	// Message Loop
	MSG msg{ NULL };
	while (msg.message != WM_QUIT)
	{
		g_timer.Tick();
		Input::ResetMoved();

		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) > 0)
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		g_SceneController.Update(static_cast<float>(g_timer.GetDeltaTime()));

		g_renderer.Render();
	}

	return 0;
}
