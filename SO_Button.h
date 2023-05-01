#pragma once
#include "ScreenObject.h"
#include "Input.h"

class SO_Button : public ScreenObject
{
	Input* m_pInput;
public:

	SO_Button(ImageData* sprite_pImageData, int sprite_layer, game::float2 sprite_scale = { 1.f,1.f }, game::float2 sprite_offset = { 0,0 }, float sprite_frameTime = 0);
	SO_Button(const SO_Button& button);
	virtual void Update(float deltaTime) override;
	
	void SetInput(Input* pInput);
	bool Pressed();
	bool Hovered();

};

