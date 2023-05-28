#pragma once
#include <string>
#include <vector>


class ImageLoader
{
	struct Attribute;
	struct Element;
	struct Texture;

	static std::ifstream file;
	static std::vector<Texture> vTextures;
	static bool GetElement(Element& element);

public:
	static void Load(std::string dataDirectory, std::string imageDirectory);
};
