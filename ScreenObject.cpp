#include "ScreenObject.h"


game::Float2 ScreenObject::screenRes = { 1280,720 };
float ScreenObject::px_per_su = 64.f;

ScreenObject::ScreenObject(ImageData* pImageData, int layer, game::Float2 scale, game::Float2 offset, float frameTime)
	:
	location{ 0,0 },
	sprite(&location, pImageData, frameTime, layer, scale, offset),
	pInput(nullptr)
{
}
ScreenObject::ScreenObject(const ScreenObject& screenObject)
	:
	location(screenObject.location),
	sprite(&location, screenObject.sprite),
	pInput(screenObject.pInput)
{
}

void ScreenObject::Update(float deltaTime)
{
	if (pInput)
	{
		Hovered();
	}
	sprite.Update(deltaTime);
}

bool ScreenObject::Hovered()
{
	game::Float2 size{ sprite.GetSize() };

	if (pInput->GetMouseLoc().x / px_per_su > location.x - size.x / 2.f &&
		pInput->GetMouseLoc().y / px_per_su > location.y - size.y / 2.f &&
		pInput->GetMouseLoc().x / px_per_su < location.x + size.x / 2.f &&
		pInput->GetMouseLoc().y / px_per_su < location.y + size.y / 2.f)
	{
		sprite.SetAnimation(1);
		return true;
	}
	else
	{
		sprite.SetAnimation(0);
		return false;
	}
}
bool ScreenObject::Pressed()
{
	if (Hovered() && pInput->CheckPressed(BTN_LMB))
	{
		return true;
	}
	else
	{
		return false;
	}
}

void ScreenObject::SetInput(Input* input)
{
	pInput = input;
}
void ScreenObject::SetLocaion_px(game::Float2 pixels)
{
	location.x = pixels.x / px_per_su;
	location.y = pixels.y / px_per_su;
}
void ScreenObject::SetLocaion_su(game::Float2 screenUnits)
{
	location = screenUnits;
}
void ScreenObject::SetLocation_percentage(game::Float2 percentage)
{
	location.x = screenRes.x * percentage.x / px_per_su;
	location.y = screenRes.y * percentage.y / px_per_su;
}

int ScreenObject::GetBitmapIndex()
{
	return sprite.GetBitmapIndex();
}
int ScreenObject::GetRenderLayer()
{
	return sprite.GetRenderLayer();
}
game::Rect ScreenObject::GetSourceRect()
{
	return sprite.GetSourceRect();
}
game::Rect ScreenObject::GetScreenRect()
{
	game::Float2 size{ sprite.GetSize() };

	return{
		(location.x - size.x / 2) * px_per_su,
		(location.y - size.y / 2) * px_per_su,
		(location.x + size.x / 2) * px_per_su,
		(location.y + size.y / 2) * px_per_su
	};
}