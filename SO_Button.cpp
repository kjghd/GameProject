#include "SO_Button.h"


SO_Button::SO_Button(ImageData* pImageData, int layer, game::float2 scale, game::float2 offset, float frameTime)
	:
	ScreenObject(pImageData, layer, scale, offset, frameTime),
	m_pInput(nullptr)
{
}
SO_Button::SO_Button(const SO_Button& button)
	:
	ScreenObject(button),
	m_pInput(button.m_pInput)
{
}

void SO_Button::Update(float deltaTime)
{
	if (m_pInput)
	{
		Hovered();
	}
	m_sprite.Update(deltaTime);
}

bool SO_Button::Hovered()
{
	game::float2 size{ m_sprite.GetSize() };

	if (m_pInput->GetMouseLoc().x / px_per_su > m_location.x - size.x / 2.f &&
		m_pInput->GetMouseLoc().y / px_per_su > m_location.y - size.y / 2.f &&
		m_pInput->GetMouseLoc().x / px_per_su < m_location.x + size.x / 2.f &&
		m_pInput->GetMouseLoc().y / px_per_su < m_location.y + size.y / 2.f)
	{
		m_sprite.SetAnimation(1);
		return true;
	}
	else
	{
		m_sprite.SetAnimation(0);
		return false;
	}
}
bool SO_Button::Pressed()
{
	if (Hovered() && m_pInput->CheckPressed(BTN_LMB))
	{
		return true;
	}
	else
	{
		return false;
	}
}

void SO_Button::SetInput(Input* input)
{
	m_pInput = input;
}