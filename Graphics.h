#pragma once
#include "DataTypes.h"
#include <d2d1.h>
#include <wincodec.h> // IWICImagingFactory
#include <vector>


class Graphics
{
	static ID2D1Factory* pFactory;
	static ID2D1HwndRenderTarget* pRenderTarget;
	static IWICImagingFactory* pImagingFactory;
	static std::vector<ID2D1Bitmap*> vpBitmaps;
	static float windowDPI;
	static D2D1_SIZE_U screen_resolution;
	static ID2D1SolidColorBrush* pDebugBrush;

public:
	static void Init(HWND hWnd);
	static void LoadImageFromFile(LPCWSTR filename, unsigned int texture);
	static void CreateTextureFromFile(std::string filename, size_t& index, game::int2& dimensions);
	static game::float2 GetImageSize(size_t index);
	static void BeginDraw();
	static void EndDraw();
	static void ClearScreen(D2D1_COLOR_F colour = { 0,0,0,1 });
	static void ClearScreen(float r, float g, float b, float alpha = 1.f);
	static void DrawBitmap(D2D1_RECT_F rectf, unsigned int texture, FLOAT opacity = 1.f, bool invertX = false, bool invertY = false);
	static void DrawBitmapRegion(D2D1_RECT_F rectf, unsigned int texture, D2D1_RECT_F region, FLOAT opacity = 1.f, bool invertX = false, bool invertY = false);
	static void DebugBox(D2D1_RECT_F rectf);
	static void DebugCircle(D2D1_POINT_2F location, float radius);
};
