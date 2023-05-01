#pragma once

#include "DataTypes.h"

class ImageData
{
private:
	//struct FrameRange { int start, end; };

	int m_texture;
	game::int2 m_dimensions_px;

	int m_frames;
	game::rect* m_frameRectArray;

	int m_animCount;
	game::int2* m_animRangeArray;

public:
	ImageData();
	ImageData(int texture, game::int2 dimensions_px, int frames, game::rect* frameRectArray, int animCount, game::int2* animRangeArray);
	ImageData(int texture, game::int2 dimensions_px);
	~ImageData();

	int GetTexture();
	game::int2 GetDimensionsPx();
	game::rect GetCurrentRect(int currentFrame);
	int GetAnimStartFrame(int currentAnim);
	int GetAnimEndFrame(int currentAnim);

	ImageData& operator=(const ImageData& imageData)
	{
		m_texture = imageData.m_texture;
		m_dimensions_px = imageData.m_dimensions_px;
	
		m_frames = imageData.m_frames;
	
		delete[] m_frameRectArray;
		m_frameRectArray = new game::rect[imageData.m_frames];
		for (size_t i{ 0 }; i < imageData.m_frames; ++i)
			m_frameRectArray[i] = imageData.m_frameRectArray[i];
	
		m_animCount = imageData.m_animCount;
	
		delete[] m_animRangeArray;
		m_animRangeArray = new game::int2[imageData.m_animCount];
		for (size_t i{ 0 }; i < imageData.m_animCount; ++i)
			m_animRangeArray[i] = imageData.m_animRangeArray[i];
	
		return *this;
	}
};