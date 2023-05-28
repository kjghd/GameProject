#pragma once
#include "ScreenObject.h"


class SO_Button : public ScreenObject
{
	bool hovered;
	bool pressed;

	void SetPressed();
	void SetHovered();

protected:
	virtual void WriteData(std::ostream& os) override;

public:
	SO_Button(ImageData* sprite_pImageData, int sprite_layer, game::float2 sprite_scale = { 1.f,1.f }, game::float2 sprite_offset = { 0,0 }, float sprite_frameTime = 0);
	SO_Button(const SO_Button& button);
	SO_Button(std::istream& is);

	virtual GameObject* Clone() override;

	virtual void Update(float deltaTime) override;

	bool IsPressed();
	bool IsHovered();

};
