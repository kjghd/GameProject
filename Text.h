#pragma once
#include "GameObject.h"
#include "Sprite.h"
#include <vector>
#include <string>


class Text
{
protected:
	GameObject* m_pOwner;
	game::float2 m_offset;
	bool m_centered;
	float m_fontScale;
	float m_lineSpacing;
	std::string m_text;
	std::vector<Sprite> m_vSprites;

public:
	Text(GameObject* pOwner, std::string text, float fontScale, float lineSpacing, bool centered, game::float2 offset = { 0,0 });
	Text(GameObject* pOwner, const Text& text);

	void Set(std::string text);
	void Update(float deltaTime);
};
