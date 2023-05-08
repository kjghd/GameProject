#pragma once
#include "Textures.h"
#include "Sprite.h"
#include <vector>

class ImageDataList
{
	//static ImageData list[T_COUNT];

	static std::vector<ImageData> vList;

public:
	static void Initialise();
	static void Create(ImageData imageData);
	static ImageData* Get(std::string path);
};
