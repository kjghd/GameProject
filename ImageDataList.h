#pragma once
#include "Textures.h"
#include "Sprite.h"
#include <vector>

class ImageDataList
{
	static ImageData list[T_COUNT];

public:
	static void Initialise();
	static ImageData* Get(size_t index);
};
