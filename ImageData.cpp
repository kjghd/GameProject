#include "ImageData.h"
#include "Textures.h"
#include <vector>
#include <string>

//ImageData::ImageData()
//	:
//	m_textureIndex(T_Error),
//	m_dimensions_px{ 16,16 },
//	m_frames(1),
//	m_frameRectArray(new game::rect[1]),
//	m_animCount(1),
//	m_animRangeArray(new game::int2[1])
//{
//	m_frameRectArray[0] = { 0,0,(float)m_dimensions_px.x, (float)m_dimensions_px.y };
//	m_animRangeArray[0] = { 0,0 };
//}

ImageData::ImageData(std::string filename, size_t texture, game::int2 dimensions_px, int spritesX, int spritesY, std::vector<game::Animation> vAnimRanges)
	:
	m_filename(filename),
	m_textureIndex(texture),
	m_dimensions_px(dimensions_px),
	m_frames(spritesX * spritesY),
	m_vAnimRanges(vAnimRanges)
{
	game::int2 px{
		m_dimensions_px.x / spritesX,
		m_dimensions_px.y / spritesY
	};

	for (size_t y = 0; y < spritesY; y++)
		for (size_t x = 0; x < spritesX; x++)
			m_vFrameRects.push_back(
				{
					static_cast<float>(px.x * x),
					static_cast<float>(px.y * y),
					static_cast<float>(px.x * (x + 1)),
					static_cast<float>(px.y * (y + 1))
				}
			);
}


int ImageData::GetTexture()
{
	return m_textureIndex;
}

game::int2 ImageData::GetDimensionsPx()
{
	return m_dimensions_px;
}

game::rect ImageData::GetCurrentRect(size_t currentFrame)
{
	return m_vFrameRects.at(currentFrame);
}

int ImageData::GetAnimStartFrame(size_t currentAnim)
{
	return m_vAnimRanges.at(currentAnim).range.x;
}

int ImageData::GetAnimEndFrame(size_t currentAnim)
{
	return m_vAnimRanges.at(currentAnim).range.y;
}

bool ImageData::MatchPath(std::string path)
{
	return path == m_filename;
}
