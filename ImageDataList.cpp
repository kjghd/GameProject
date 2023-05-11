#include "ImageDataList.h"
#include "DataTypes.h"
#include "ImageData.h"
#include <vector>


std::vector<ImageData> ImageDataList::vList;

void ImageDataList::Create(ImageData imageData)
{
	vList.push_back(imageData);
}

ImageData* ImageDataList::Get(std::string path)
{
	for (auto& imageData : vList)
		if (imageData.MatchPath(path))
		{
			return &imageData;
		}

	// Error
	for (auto& imageData : vList)
		if (imageData.MatchPath("Data/Textures/Error.png"))
		{
			return &imageData;
		}

	return nullptr;
}
