#pragma once
#include "DataTypes.h"
#include <vector>


class ImageData
{
private:
	std::string m_filename;
	std::vector<game::rect> m_vFrameRects;
	std::vector<game::Animation> m_vAnimRanges;
	size_t m_textureIndex;
	game::int2 m_dimensions_px;
	int m_frames;

public:
	ImageData(std::string filename, size_t texture, game::int2 dimensions_px, int spritesX, int spritesY, std::vector<game::Animation> vAnimRanges);
	int GetTexture();
	game::int2 GetDimensionsPx();
	game::rect GetCurrentRect(size_t currentFrame);
	int GetAnimStartFrame(size_t currentAnim);
	int GetAnimEndFrame(size_t currentAnim);
	bool MatchFilename(std::string);
	std::string GetFilename();
};
