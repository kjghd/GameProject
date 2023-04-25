#include "ScreenObject.h"


game::Float2 ScreenObject::screenRes = { 1280,720 };
float ScreenObject::px_per_su = 64.f;

ScreenObject::ScreenObject(ImageData* pImageData, int layer, game::Float2 scale, game::Float2 offset, float frameTime)
	:
	GameObject(pImageData, layer, scale, offset, frameTime)
{
}
ScreenObject::ScreenObject(const ScreenObject& screenObject)
	:
	GameObject(screenObject)
{
}

void ScreenObject::Update(float deltaTime)
{
	m_sprite.Update(deltaTime);
}

void ScreenObject::SetLocaion_px(game::Float2 pixels)
{
	m_location.x = pixels.x / px_per_su;
	m_location.y = pixels.y / px_per_su;
}
void ScreenObject::SetLocaion_su(game::Float2 screenUnits)
{
	m_location = screenUnits;
}
void ScreenObject::SetLocation_percentage(game::Float2 percentage)
{
	m_location.x = screenRes.x * percentage.x / px_per_su;
	m_location.y = screenRes.y * percentage.y / px_per_su;
}

int ScreenObject::GetBitmapIndex()
{
	return m_sprite.GetBitmapIndex();
}
int ScreenObject::GetRenderLayer()
{
	return m_sprite.GetRenderLayer();
}
game::Rect ScreenObject::GetSourceRect()
{
	return m_sprite.GetSourceRect();
}
game::Rect ScreenObject::GetScreenRect()
{
	game::Float2 size{ m_sprite.GetSize() };

	return{
		(m_location.x - size.x / 2) * px_per_su,
		(m_location.y - size.y / 2) * px_per_su,
		(m_location.x + size.x / 2) * px_per_su,
		(m_location.y + size.y / 2) * px_per_su
	};
}

bool ScreenObject::InvertedX()
{
	return m_sprite.CheckInvertedX();
}
bool ScreenObject::InvertedY()
{
	return m_sprite.CheckInvertedY();
}

bool ScreenObject::CompareRenderOrder_Under(ScreenObject* screenObjectA, ScreenObject* screenObjectB)
{
	return screenObjectA->GetRenderLayer() > screenObjectB->GetRenderLayer();
}