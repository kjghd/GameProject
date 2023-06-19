#pragma once
#include "ScreenObject.h"


class SO_TextBox : public ScreenObject
{
protected:
	bool m_centered;
	float m_fontsize;
	float m_linespacing;
	std::string m_text;
	std::vector<Sprite> m_vTextSprites;

public:
	SO_TextBox(std::string text, float fontsize, float linespacing, bool centered, ImageData* sprite_pImageData, int sprite_layer, game::float2 sprite_scale = { 1.f,1.f }, game::float2 sprite_offset = { 0,0 }, float sprite_frameTime = 0);
	SO_TextBox(const SO_TextBox& textbox);

	virtual void Update(float deltaTime);

	void SetText(std::string text);

	virtual GameObject* Clone();
};
