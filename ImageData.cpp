#include "ImageData.h"
#include "Textures.h"


ImageData::ImageData()
	:
	m_texture(T_Error),
	m_dimensions_px{ 16,16 },
	m_frames(1),
	m_frameRectArray(new game::rect[1]),
	m_animCount(1),
	m_animRangeArray(new game::int2[1])
{
	m_frameRectArray[0] = { 0,0,(float)m_dimensions_px.x, (float)m_dimensions_px.y };
	m_animRangeArray[0] = { 0,0 };
}

ImageData::ImageData(int texture, game::int2 dimensions_px, int frames, game::rect* frameRectArray, int animCount, game::int2* animRangeArray)
	:
	m_texture(texture),
	m_dimensions_px(dimensions_px),
	m_frames(frames),
	m_frameRectArray(new game::rect[frames]),
	m_animCount(animCount),
	m_animRangeArray(new game::int2[animCount])
{
	for (size_t i{ 0 }; i < frames; ++i)
		m_frameRectArray[i] = frameRectArray[i];

	for (size_t i{ 0 }; i < animCount; ++i)
		m_animRangeArray[i] = animRangeArray[i];
}

ImageData::ImageData(int texture, game::int2 dimensions_px)
	:
	m_texture(texture),
	m_dimensions_px(dimensions_px),
	m_frames(1),
	m_frameRectArray(new game::rect[1]),
	m_animCount(1),
	m_animRangeArray(new game::int2[1])
{
	m_frameRectArray[0] = game::rect{ 0,0,(float)dimensions_px.x, (float)dimensions_px.y };
	m_animRangeArray[0] = game::int2{ 0,0 };
}

ImageData::~ImageData()
{
	delete[] m_frameRectArray;
	delete[] m_animRangeArray;
}

int ImageData::GetTexture()
{
	return m_texture;
}

game::int2 ImageData::GetDimensionsPx()
{
	return m_dimensions_px;
}

game::rect ImageData::GetCurrentRect(int currentFrame)
{
	return m_frameRectArray[currentFrame];
}

int ImageData::GetAnimStartFrame(int currentAnim)
{
	return m_animRangeArray[currentAnim].x;
}

int ImageData::GetAnimEndFrame(int currentAnim)
{
	return m_animRangeArray[currentAnim].y;
}
