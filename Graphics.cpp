#include "Graphics.h"
#include "Textures.h"

#pragma comment(lib, "d2d1")


void Graphics::Init(HWND hWnd)
{
	CoInitializeEx(NULL, COINIT_APARTMENTTHREADED);
	CoCreateInstance(CLSID_WICImagingFactory, NULL, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&pImagingFactory));

	D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &pFactory);

	RECT rc;
	GetClientRect(hWnd, &rc);
	D2D1_SIZE_U size{
		static_cast<UINT32>(rc.right - rc.left),
		static_cast<UINT32>(rc.bottom - rc.top)
	};

	pFactory->CreateHwndRenderTarget(
		D2D1::RenderTargetProperties(),
		D2D1::HwndRenderTargetProperties(hWnd, size),
		&pRenderTarget
	);

	// Load Images
	vpBitmaps.reserve(T_COUNT);
	vpBitmaps.resize(T_COUNT);

	LoadImageFromFile(L"Data/Textures/Error.png", T_Error);
	LoadImageFromFile(L"Data/Textures/Shadow.png", T_Shadow);
	LoadImageFromFile(L"Data/Textures/Wallace.png", T_Wallace);

	LoadImageFromFile(L"Data/Textures/Block_Blue.png", T_Blue);
	LoadImageFromFile(L"Data/Textures/Block_Green.png", T_Green);
	LoadImageFromFile(L"Data/Textures/Block_Red.png", T_Red);
	LoadImageFromFile(L"Data/Textures/Block_Yellow.png", T_Yellow);

	LoadImageFromFile(L"Data/Textures/Ball_Blue.png", T_BallBlue);
	LoadImageFromFile(L"Data/Textures/Ball_Green.png", T_BallGreen);
	LoadImageFromFile(L"Data/Textures/Ball_Red.png", T_BallRed);
	LoadImageFromFile(L"Data/Textures/Ball_Yellow.png", T_BallYellow);



	windowDPI = GetDpiForWindow(hWnd);

	pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(1, 1, 1), &pDebugBrush);
}

void Graphics::LoadImageFromFile(LPCWSTR filename, unsigned int texture)
{
	// Create a decoder
	IWICBitmapDecoder* pDecoder;
	pImagingFactory->CreateDecoderFromFilename(
		filename,
		NULL,
		GENERIC_READ,
		WICDecodeMetadataCacheOnDemand,
		&pDecoder
	);

	// Create a source by getting the first frame of the decoder
	IWICBitmapFrameDecode* pFrameDecode;
	pDecoder->GetFrame(0, &pFrameDecode);

	// Convert the format to something direct2d can use
	IWICFormatConverter* pConverter;
	pImagingFactory->CreateFormatConverter(&pConverter);
	pConverter->Initialize(
		pFrameDecode,
		GUID_WICPixelFormat32bppPBGRA,
		WICBitmapDitherTypeNone,
		NULL,
		0.f,
		WICBitmapPaletteTypeMedianCut
	);

	// convert the wic source into a direct 2d bitmap
	pRenderTarget->CreateBitmapFromWicBitmap(pConverter, &vpBitmaps.at(texture));
}

void Graphics::BeginDraw()
{
	pRenderTarget->BeginDraw();
}
void Graphics::EndDraw()
{
	pRenderTarget->EndDraw();
}

void Graphics::ClearScreen(D2D1_COLOR_F colour)
{
	pRenderTarget->Clear(colour);
}

void Graphics::DrawBitmap(D2D1_RECT_F rectf, unsigned int texture)
{
	D2D1_MATRIX_3X2_F transform;
	D2D1_RECT_F adjustedRect{
		rectf.left / (windowDPI / 96.f),
		rectf.top / (windowDPI / 96.f),
		rectf.right / (windowDPI / 96.f),
		rectf.bottom / (windowDPI / 96.f)
	};

	pRenderTarget->DrawBitmap(vpBitmaps.at(texture), adjustedRect, 1.f, D2D1_BITMAP_INTERPOLATION_MODE_NEAREST_NEIGHBOR);
}

void Graphics::DebugCircle(D2D1_POINT_2F location, float radius)
{
	D2D1_POINT_2F adjustedLoc{
		location.x / (windowDPI / 96.f),
		location.y / (windowDPI / 96.f)
	};
	float adjustedRadius{ radius / (windowDPI / 96.f) };
	pRenderTarget->DrawEllipse(D2D1::Ellipse(adjustedLoc, adjustedRadius, adjustedRadius), pDebugBrush);
}
