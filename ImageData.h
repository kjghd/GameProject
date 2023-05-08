#pragma once

#include "DataTypes.h"
#include <vector>

class ImageData
{
private:
	//struct FrameRange { int start, end; };
	std::string m_filename;

	std::vector<game::rect> m_vFrameRects;
	std::vector<game::Animation> m_vAnimRanges;

	size_t m_textureIndex;
	game::int2 m_dimensions_px;

	int m_frames;
	//game::rect* m_frameRectArray;
   
	//int m_animCount;
	//game::int2* m_animRangeArray;

public:
	//ImageData();
	ImageData(std::string filename, size_t texture, game::int2 dimensions_px, int spritesX, int spritesY, std::vector<game::Animation> vAnimRanges);
	//ImageData(size_t texture, game::int2 dimensions_px, int frames, game::rect* frameRectArray, int animCount, game::int2* animRangeArray);
	//ImageData(size_t texture, game::int2 dimensions_px);
	//~ImageData();

	int GetTexture();
	game::int2 GetDimensionsPx();
	game::rect GetCurrentRect(size_t currentFrame);
	int GetAnimStartFrame(size_t currentAnim);
	int GetAnimEndFrame(size_t currentAnim);

	//ImageData& operator=(const ImageData& imageData)
	//{
	//	m_textureIndex = imageData.m_textureIndex;
	//	m_dimensions_px = imageData.m_dimensions_px;
	//
	//	m_frames = imageData.m_frames;
	//
	//	delete[] m_frameRectArray;
	//	m_frameRectArray = new game::rect[imageData.m_frames];
	//	for (size_t i{ 0 }; i < imageData.m_frames; ++i)
	//		m_frameRectArray[i] = imageData.m_frameRectArray[i];
	//
	//	m_animCount = imageData.m_animCount;
	//
	//	delete[] m_animRangeArray;
	//	m_animRangeArray = new game::int2[imageData.m_animCount];
	//	for (size_t i{ 0 }; i < imageData.m_animCount; ++i)
	//		m_animRangeArray[i] = imageData.m_animRangeArray[i];
	//
	//	return *this;
	//}

	bool MatchPath(std::string);
};