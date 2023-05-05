#include "SO_Button.h"

#include "Input.h"


SO_Button::SO_Button(ImageData* pImageData, int layer, game::float2 scale, game::float2 offset, float frameTime)
	:
	ScreenObject(pImageData, layer, scale, offset, frameTime)
{
}
SO_Button::SO_Button(const SO_Button& button)
	:
	ScreenObject(button)
{
}

void SO_Button::Update(float deltaTime)
{
	Hovered();
	m_sprite.Update(deltaTime);
}

bool SO_Button::Hovered()
{
	game::float2 size{ m_sprite.GetSize() };
	game::rect rc{ GetScreenRect() };

	game::float2 mouseLoc{
		(Input::GetMouseLoc().x - screenRes.x) / px_per_su,
		-((Input::GetMouseLoc().y - screenRes.y) / px_per_su)
	};

	if (mouseLoc.x > rc.l &&
		mouseLoc.y > rc.b &&
		mouseLoc.x < rc.r &&
		mouseLoc.y < rc.t)
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
	if (Hovered() && Input::CheckPressed(BTN_LMB))
	{
		return true;
	}
	else
	{
		return false;
	}
}
