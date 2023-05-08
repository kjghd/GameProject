#pragma once
#include <string>
#include <vector>

class ImageLoader
{
	struct Attribute;
	struct Element;

	//struct Animation;
	struct Texture;

	static std::ifstream file;
	static std::vector<Texture> vTextures;
	static Element& GetElement();


public:

	static void Load(std::string directory);
};
