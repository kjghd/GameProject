#pragma once

#include <d2d1.h>
#include <wincodec.h>
#include <vector>


class Graphics
{
	ID2D1Factory* pFactory;
	ID2D1HwndRenderTarget* pRenderTarget;

	IWICImagingFactory* pImagingFactory;

	std::vector<ID2D1Bitmap*> vpBitmaps;

	float windowDPI;
	D2D1_SIZE_U screen_resolution;

	ID2D1SolidColorBrush* pDebugBrush;

public:
	Graphics()
		:
		pFactory(NULL),
		pRenderTarget(NULL),
		pImagingFactory(NULL),
		vpBitmaps(NULL),
		windowDPI(96.f),
		screen_resolution({ 640,480 }),
		pDebugBrush(NULL)
	{
	}

	void Init(HWND hWnd);

	void LoadImageFromFile(LPCWSTR filename, unsigned int texture);

	void BeginDraw();
	void EndDraw();

	void ClearScreen(D2D1_COLOR_F colour = { 0,0,0,1 });

	void DrawBitmap(D2D1_RECT_F rectf, unsigned int texture, FLOAT opacity = 1.f, bool invertX = false, bool invertY = false);
	void DrawBitmapRegion(D2D1_RECT_F rectf, unsigned int texture, D2D1_RECT_F region, FLOAT opacity = 1.f, bool invertX = false, bool invertY = false);

	void DebugBox(D2D1_RECT_F rectf);
	void DebugCircle(D2D1_POINT_2F location, float radius);
};



