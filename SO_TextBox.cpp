#include "SO_TextBox.h"
#include "ImageDataList.h"


SO_TextBox::SO_TextBox(std::string text, float fontsize, float linespacing, bool centered, ImageData* sprite_pImageData, int sprite_layer, game::float2 sprite_scale, game::float2 sprite_offset, float sprite_frameTime)
	:
	ScreenObject(sprite_pImageData, sprite_layer, sprite_scale, sprite_offset, sprite_frameTime),
	m_centered(centered),
	m_fontsize(fontsize),
	m_linespacing(linespacing),
	m_text()
{
	SetText(text);
	m_tag = "SOTB";
}
SO_TextBox::SO_TextBox(const SO_TextBox& textbox)
	:
	ScreenObject(textbox),
	m_centered(textbox.m_centered),
	m_fontsize(textbox.m_fontsize),
	m_linespacing(textbox.m_linespacing),
	m_text()
{
	SetText(textbox.m_text);
	m_tag = "SOTB";
}

void SO_TextBox::Update(float deltaTime)
{
	m_sprite.Update(deltaTime);
	for (auto& sprite : m_vTextSprites)
		sprite.Update(deltaTime);
}

void SO_TextBox::SetText(std::string text)
{
	m_text = text;
	m_vTextSprites.clear();
	m_vTextSprites.reserve(text.length());

	int line{ 0 };
	int col{ 0 };
	int longestLine{ 0 };

	for (size_t i = 0; i < m_text.length(); i++)
	{
		if (m_text.at(i) == '\n')
		{
			++line;
			col = 0;
		}

		if (m_text.at(i) >= ' ' && m_text.at(i) <= '~')
		{
			m_vTextSprites.push_back(
				Sprite(
					this,
					ImageDataList::Get("font_32x48.png"),
					0,
					SL_UI_Front,
					{ m_fontsize,m_fontsize },
					{ col * m_fontsize, -line * m_fontsize * m_linespacing }
				)
			);
			m_vTextSprites.back().SetFrame(text.at(i));

			++col;
		}
	}

	m_sprite.visible = false;

	
}

GameObject* SO_TextBox::Clone()
{
	return new SO_TextBox(*this);
}