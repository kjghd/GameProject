#pragma once
#include "ScreenObject.h"


class SO_Button : public ScreenObject
{
	bool hovered;
	bool pressed;

	void SetPressed();
	void SetHovered();

public:
	SO_Button(ImageData* sprite_pImageData, int sprite_layer, game::float2 sprite_scale = { 1.f,1.f }, game::float2 sprite_offset = { 0,0 }, float sprite_frameTime = 0);
	SO_Button(const SO_Button& button);

	virtual void Update(float deltaTime) override;

	bool IsPressed();
	bool IsHovered();
};
