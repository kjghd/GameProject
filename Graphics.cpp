#include "Graphics.h"
#include "Textures.h"
#pragma comment(lib, "d2d1")


ID2D1Factory* Graphics::pFactory;
ID2D1HwndRenderTarget* Graphics::pRenderTarget;
IWICImagingFactory*	Graphics::pImagingFactory;
std::vector<ID2D1Bitmap*> Graphics::vpBitmaps;
float Graphics::windowDPI;
D2D1_SIZE_U	Graphics::screen_resolution;
ID2D1SolidColorBrush* Graphics::pDebugBrush;


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
	//vpBitmaps.reserve(T_COUNT);
	//vpBitmaps.resize(T_COUNT);
	//
	//LoadImageFromFile(L"Data/Textures/Error.png", T_Error);
	//LoadImageFromFile(L"Data/Textures/Wallace.png", T_Wallace);
	//LoadImageFromFile(L"Data/Textures/Mushroom.png", T_Mushroom);
	//
	//LoadImageFromFile(L"Data/Textures/font_32x48.png", T_Font);
	//
	//LoadImageFromFile(L"Data/Textures/sprite_sheet.png", T_Animation);
	//LoadImageFromFile(L"Data/Textures/guy_sheet.png", T_Guy);
	//LoadImageFromFile(L"Data/Textures/guy_sheet2.png", T_Guy2);
	//
	//LoadImageFromFile(L"Data/Textures/Block_Blue.png", T_Blue);
	//LoadImageFromFile(L"Data/Textures/Block_Green.png", T_Green);
	//LoadImageFromFile(L"Data/Textures/Block_Red.png", T_Red);
	//
	//LoadImageFromFile(L"Data/Textures/Ball_Blue.png", T_BallBlue);
	//LoadImageFromFile(L"Data/Textures/Ball_Green.png", T_BallGreen);
	//LoadImageFromFile(L"Data/Textures/Ball_Red.png", T_BallRed);
	//
	//LoadImageFromFile(L"Data/Textures/shopping_floor.png", T_Floor);
	//
	//LoadImageFromFile(L"Data/Textures/Cursor.png", T_UI_Cursor);
	//LoadImageFromFile(L"Data/Textures/CursorBlinking.png", T_UI_CursorBlinking);
	//LoadImageFromFile(L"Data/Textures/TileSelection.png", T_UI_Selection);
	//LoadImageFromFile(L"Data/Textures/Resume.png", T_UI_Resume);
	//LoadImageFromFile(L"Data/Textures/MainMenu.png", T_UI_MainMenu);
	//LoadImageFromFile(L"Data/Textures/BG.png", T_UI_BG);


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
void Graphics::CreateTextureFromFile(std::string filename, size_t& index, game::int2& dimesnions)
{
	size_t uConverted;
	wchar_t* wzFilename = new wchar_t[filename.size()];
	mbstowcs_s(&uConverted, wzFilename, filename.size() + 1, filename.c_str(), filename.size());

	// Create a decoder
	IWICBitmapDecoder* pDecoder;
	pImagingFactory->CreateDecoderFromFilename(
		wzFilename,
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
	vpBitmaps.resize(vpBitmaps.size() + 1);
	pRenderTarget->CreateBitmapFromWicBitmap(pConverter, &vpBitmaps.back());
	
	index = vpBitmaps.size() - 1;
	D2D1_SIZE_U size{ vpBitmaps.back()->GetPixelSize() };
	dimesnions.x = size.width;
	dimesnions.y = size.height;

	//delete[] wzFilename;
	//wzFilename = nullptr;
}

game::float2 Graphics::GetImageSize(size_t index)
{
	D2D1_SIZE_U result{ vpBitmaps.at(index)->GetPixelSize() };
	return { static_cast<float>(result.width), static_cast<float>(result.height) };
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
void Graphics::ClearScreen(float r, float g, float b, float alpha)
{
	pRenderTarget->Clear(D2D1::ColorF(r,g,b,alpha));
}

void Graphics::DrawBitmap(D2D1_RECT_F rectf, unsigned int texture, FLOAT opacity, bool invertX, bool invertY)
{
	D2D1_RECT_F adjustedRect{
		rectf.left / (windowDPI / 96.f),
		rectf.top / (windowDPI / 96.f),
		rectf.right / (windowDPI / 96.f),
		rectf.bottom / (windowDPI / 96.f)
	};

	if (invertX || invertY)
	{
		D2D1_POINT_2F center{
			adjustedRect.left + (adjustedRect.right - adjustedRect.left) / 2,
			adjustedRect.top + (adjustedRect.bottom - adjustedRect.top) / 2
		};
		D2D1_SIZE_F scale{
			invertX ? -1 : 1,
			invertY ? -1 : 1
		};
		pRenderTarget->SetTransform(D2D1::Matrix3x2F::Scale(scale, center));
	}

	pRenderTarget->DrawBitmap(vpBitmaps.at(texture), adjustedRect, opacity, D2D1_BITMAP_INTERPOLATION_MODE_NEAREST_NEIGHBOR);

	pRenderTarget->SetTransform(D2D1::Matrix3x2F{ 1,0,0,1,0,0 });
}
void Graphics::DrawBitmapRegion(D2D1_RECT_F rectf, unsigned int texture, D2D1_RECT_F region, FLOAT opacity, bool invertX, bool invertY)
{
	D2D1_RECT_F adjustedRect{
		rectf.left	 / (windowDPI / 96.f),
		rectf.top    / (windowDPI / 96.f),
		rectf.right  / (windowDPI / 96.f),
		rectf.bottom / (windowDPI / 96.f)
	};

	if (invertX || invertY)
	{
		D2D1_POINT_2F center{
			adjustedRect.left + (adjustedRect.right - adjustedRect.left) / 2,
			adjustedRect.top + (adjustedRect.bottom - adjustedRect.top) / 2
		};
		D2D1_SIZE_F scale{
			invertX ? -1 : 1,
			invertY ? -1 : 1
		};
		pRenderTarget->SetTransform(D2D1::Matrix3x2F::Scale(scale, center));
	}

	pRenderTarget->DrawBitmap(vpBitmaps.at(texture), adjustedRect, opacity, D2D1_BITMAP_INTERPOLATION_MODE_NEAREST_NEIGHBOR, region);

	pRenderTarget->SetTransform(D2D1::Matrix3x2F{ 1,0,0,1,0,0 });
}
void Graphics::DebugBox(D2D1_RECT_F rectf)
{
	D2D1_RECT_F adjustedRect{
		rectf.left / (windowDPI / 96.f),
		rectf.top / (windowDPI / 96.f),
		rectf.right / (windowDPI / 96.f),
		rectf.bottom / (windowDPI / 96.f)
	};

	pRenderTarget->DrawRectangle(adjustedRect, pDebugBrush);
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