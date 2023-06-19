#pragma once
#include "ScreenObject.h"
#include "Text.h"

class SO_Button : public ScreenObject
{
	bool hovered;
	bool pressed;

	void SetPressed();
	void SetHovered();

	Text m_text;

protected:
	virtual void WriteData(std::ostream& os) override;

public:
	SO_Button(std::string text);
	SO_Button(const SO_Button& button);
	SO_Button(std::istream& is);

	virtual GameObject* Clone() override;

	virtual void Update(float deltaTime) override;

	bool IsPressed();
	bool IsHovered();

};
