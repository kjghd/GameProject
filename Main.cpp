#include "Graphics.h"
#include "Renderer.h"
#include "Timer.h"
#include "Input.h"
#include "Scene.h"

#include <Windows.h>
#include <Windowsx.h>


// Globals.
static Graphics g_graphics;
static Renderer g_renderer;
static Timer g_timer;
static Input g_input;
static Scene g_scene(&g_input);

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
		g_input.SetMouseLoc(
			D2D1::Point2F(
				static_cast<FLOAT>(GET_X_LPARAM(lParam)),
				static_cast<FLOAT>(GET_Y_LPARAM(lParam))
			)
		);
	}
	else if (uMsg == WM_LBUTTONDOWN) g_input.SetOn(BTN_LMB);
	else if (uMsg == WM_RBUTTONDOWN) g_input.SetOn(BTN_RMB);
	else if (uMsg == WM_LBUTTONUP) g_input.SetOff(BTN_LMB);
	else if (uMsg == WM_RBUTTONUP) g_input.SetOff(BTN_RMB);
	else if (uMsg == WM_MOUSEWHEEL) g_input.SetScrollDistance(GET_WHEEL_DELTA_WPARAM(wParam));

	// Keyboard
	else if (uMsg == WM_KEYDOWN)
	{
		switch (wParam)
		{
		case 0x30: g_input.SetOn(BTN_0); break;
		case 0x31: g_input.SetOn(BTN_1); break;
		case 0x32: g_input.SetOn(BTN_2); break;
		case 0x33: g_input.SetOn(BTN_3); break;
		case 0x34: g_input.SetOn(BTN_4); break;
		case 0x35: g_input.SetOn(BTN_5); break;
		case 0x36: g_input.SetOn(BTN_6); break;
		case 0x37: g_input.SetOn(BTN_7); break;
		case 0x38: g_input.SetOn(BTN_8); break;
		case 0x39: g_input.SetOn(BTN_9); break;

		case 0x41: g_input.SetOn(BTN_A); break;
		case 0x44: g_input.SetOn(BTN_D); break;
		case 0x45: g_input.SetOn(BTN_E); break;
		case 0x51: g_input.SetOn(BTN_Q); break;
		case 0x53: g_input.SetOn(BTN_S); break;
		case 0x57: g_input.SetOn(BTN_W); break;

		case VK_SHIFT: g_input.SetOn(BTN_SHIFT); break;
		case VK_TAB: g_input.SetOn(BTN_TAB); break;
		case VK_ESCAPE: g_input.SetOn(BTN_ESC); break;
		}
	}
	else if (uMsg == WM_KEYUP)
	{
		switch (wParam)
		{
		case 0x30: g_input.SetOff(BTN_0); break;
		case 0x31: g_input.SetOff(BTN_1); break;
		case 0x32: g_input.SetOff(BTN_2); break;
		case 0x33: g_input.SetOff(BTN_3); break;
		case 0x34: g_input.SetOff(BTN_4); break;
		case 0x35: g_input.SetOff(BTN_5); break;
		case 0x36: g_input.SetOff(BTN_6); break;
		case 0x37: g_input.SetOff(BTN_7); break;
		case 0x38: g_input.SetOff(BTN_8); break;
		case 0x39: g_input.SetOff(BTN_9); break;

		case 0x41: g_input.SetOff(BTN_A); break;
		case 0x44: g_input.SetOff(BTN_D); break;
		case 0x45: g_input.SetOff(BTN_E); break;
		case 0x51: g_input.SetOff(BTN_Q); break;
		case 0x53: g_input.SetOff(BTN_S); break;
		case 0x57: g_input.SetOff(BTN_W); break;

		case VK_SHIFT: g_input.SetOff(BTN_SHIFT); break;
		case VK_TAB: g_input.SetOff(BTN_TAB); break;
		case VK_ESCAPE: g_input.SetOff(BTN_ESC); break;
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
	int width  = 1280;
	int height = 720;
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

	// Start timer
	g_graphics.Init(hWnd);
	g_renderer.Init(&g_graphics, &g_scene);
	g_timer.Start();
	Camera::m_screenResolution = { width,height };

	// Message Loop
	MSG msg{ NULL };
	while (msg.message != WM_QUIT)
	{
		g_timer.Tick();
		g_input.ResetMoved();

		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) > 0)
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		g_scene.Update(static_cast<float>(g_timer.deltaTime));

		g_renderer.Render();
	}

	return 0;
}