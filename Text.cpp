#include "Text.h"
#include "ImageDataList.h"


Text::Text(GameObject* pOwner, std::string text, float fontScale, float lineSpacing, bool centered, game::float2 offset)
	:
	m_pOwner(pOwner),
	m_offset(offset),
	m_centered(centered),
	m_fontScale(fontScale),
	m_lineSpacing(lineSpacing),
	m_text()
{
	Set(text);
}
Text::Text(GameObject* pOwner, const Text& text)
	:
	m_pOwner(pOwner),
	m_offset(text.m_offset),
	m_centered(text.m_centered),
	m_fontScale(text.m_fontScale),
	m_lineSpacing(text.m_lineSpacing),
	m_text()
{
	Set(text.m_text);
}

void Text::Set(std::string text)
{
	m_text = text;
	m_vSprites.clear();
	m_vSprites.reserve(text.length());

	int line{ 0 };
	int col{ 0 };

	for (size_t i = 0; i < m_text.length(); i++)
	{
		//if (m_text.at(i) == '\n')
		//{
		//	++line;
		//	col = 0;
		//}

		float offset{ m_centered ? (m_text.length() - 1) * m_fontScale / 2.f : 0 };

		if (m_text.at(i) >= ' ' && m_text.at(i) <= '~')
		{
			m_vSprites.push_back(
				Sprite(
					m_pOwner,
					ImageDataList::Get("font_32x48.png"),
					0,
					SL_UI_Front,
					{ m_fontScale,m_fontScale },
					{ col * m_fontScale - offset , -line * m_fontScale * m_lineSpacing}
				)
			);
			m_vSprites.back().SetFrame(text.at(i));
			++col;
		}
	}
}

void Text::Update(float deltaTime)
{
	for (auto& sprite : m_vSprites)
		sprite.Update(deltaTime);
}