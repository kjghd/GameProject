#include "SO_Button.h"
#include "Input.h"


SO_Button::SO_Button(ImageData* pImageData, int layer, game::float2 scale, game::float2 offset, float frameTime)
	:
	ScreenObject(pImageData, layer, scale, offset, frameTime),
	hovered(false),
	pressed(false)
{
	m_tag = "SOBT";
}
SO_Button::SO_Button(const SO_Button& button)
	:
	ScreenObject(button),
	hovered(false),
	pressed(false)
{
	m_tag = "SOBT";
}
SO_Button::SO_Button(std::istream& is)
	:
	ScreenObject(is),
	hovered(FileWritable::GetNextValue(is) == "1" ? true : false),
	pressed(FileWritable::GetNextValue(is) == "1" ? true : false)
{
	m_tag = "SOBT";
}

GameObject* SO_Button::Clone()
{
	return new SO_Button(*this);
}

void SO_Button::Update(float deltaTime)
{
	hovered = false;
	SetHovered();

	pressed = false;
	SetPressed();

	m_sprite.Update(deltaTime);
}

void SO_Button::SetHovered()
{
	game::rect rc{ GetScreenRect() };

	game::float2 mouseLoc{
		(Input::GetMouseLoc().x - ScreenObject::screenRes.x / 2.f) / ScreenObject::px_per_su,
		-(Input::GetMouseLoc().y - ScreenObject::screenRes.y / 2.f) / ScreenObject::px_per_su
	};

	if (mouseLoc.x > rc.l &&
		mouseLoc.y > rc.b &&
		mouseLoc.x < rc.r &&
		mouseLoc.y < rc.t)
	{
		m_sprite.SetAnimation("on");
		hovered = true;
	}
	else
	{
		m_sprite.SetAnimation("off");
		hovered = false;
	}
}
void SO_Button::SetPressed()
{
	if (hovered && Input::CheckPressed(BTN_LMB))
	{
		pressed = true;
	}
	else
	{
		pressed = false;
	}
}

bool SO_Button::IsPressed()
{
	return pressed;
}

bool SO_Button::IsHovered()
{
	return hovered;
}

void SO_Button::WriteData(std::ostream& os)
{
	ScreenObject::WriteData(os);
	os << ',';
	os << hovered << ',';
	os << pressed;
}