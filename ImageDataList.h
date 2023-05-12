#pragma once
#include <vector>
#include <string>


class ImageData;

class ImageDataList
{
	static std::vector<ImageData> vList;

public:
	static void Initialise();
	static void Create(ImageData imageData);
	static ImageData* Get(std::string path);
};
