#include "ImageLoader.h"
#include "DataTypes.h"
#include "Graphics.h"
#include "ImageDataList.h"
#include <fstream>


struct ImageLoader::Attribute {
	std::string tag;
	std::string content;

	bool operator!=(Attribute a)
	{
		return tag != a.tag && content != a.content;
	}
};
struct ImageLoader::Element {
	std::string tag;
	std::vector<Attribute> vAttributes;
	bool open;

	bool operator==(Element e)
	{
		if (vAttributes.size() == e.vAttributes.size())
		{
			bool equal{ tag == e.tag };
			for (size_t i{ 0 }; i < vAttributes.size(); ++i)
			{
				if (vAttributes.at(i) != e.vAttributes.at(i))
					equal = false;
			}
			return equal;
		}
		else
			return false;
	}
};

//struct ImageLoader::Animation
//{
//	std::string name;
//	game::int2 range;
//};
struct ImageLoader::Texture
{
	std::string fileLocation;
	game::int2 spriteCount;
	std::vector<game::Animation> vAnimations;

};

std::ifstream ImageLoader::file;
std::vector<ImageLoader::Texture> ImageLoader::vTextures;


bool ImageLoader::GetElement(ImageLoader::Element& e)
{
	bool open{ true };
	if (file.peek() == '<')
	{
		file.ignore(1); // '<'

		if (file.peek() == '/')
		{
			open = false;
			file.ignore(1); // '/'
		}

		// Get Tag.
		while (file.peek() != ' ' && file.peek() != '>' && file.peek() != '/')
			e.tag.push_back(file.get());

		int n = file.peek();  
		// Get Attributes.
		while (file.peek() == ' ')
		{
			file.ignore(1); // ' '

			Attribute a;

			while (file.peek() != '=')
				a.tag.push_back(file.get());

			file.ignore(2); // '="'

			while (file.peek() != '\"')
				a.content.push_back(file.get());

			e.vAttributes.push_back(a);
			file.ignore(1); // '"'
		}

		// End of Tag.
		if (file.peek() == '/')
		{
			open = false;
			file.ignore(1);
		}
		if (file.peek() == '>')
		{
			e.open = open;
			file.ignore(1); // '>'
		}

		return true;
	}
	else return false;
}


void ImageLoader::Load(std::string dataDirectory)
{
	// Parse file.
	file.open(dataDirectory);
	bool writingTexture{ true };
	while (file.peek() != EOF)
	{
		while (file.peek() == '\n' || file.peek() == '\t')
		{
			file.ignore(1);
		}

		if (file.peek() == '<')
		{
			Element e;
			GetElement(e);

			if (e.tag == "Texture" && e.open)
			{
				writingTexture = true;
				vTextures.push_back(Texture());
				for (const auto& a : e.vAttributes)
					if (a.tag == "location")
						vTextures.back().fileLocation = a.content;
			}
			else if (writingTexture)
			{
				if (e.tag == "Sprites" && !e.open)
				{
					for (const auto& a : e.vAttributes)
						if (a.tag == "count")
						{
							std::string x;
							std::string y;

							size_t pos{ a.content.find(',') };
							x = a.content.substr(0, pos);
							y = a.content.substr(pos + 1, a.content.size() - 1);

							vTextures.back().spriteCount.x = std::stoi(x);
							vTextures.back().spriteCount.y = std::stoi(y);
						}
				}
				if (e.tag == "Animation" && !e.open)
				{
					vTextures.back().vAnimations.push_back(game::Animation());
					for (const auto& a : e.vAttributes)
					{
						if (a.tag == "name")
						{
							vTextures.back().vAnimations.back().name = a.content;
						}
						else if (a.tag == "range")
						{
							std::string first;
							std::string last;

							size_t pos{ a.content.find(',') };
							first = a.content.substr(0, pos);
							last = a.content.substr(pos + 1, a.content.size() - 1);

							vTextures.back().vAnimations.back().range.x = std::stoi(first);
							vTextures.back().vAnimations.back().range.y = std::stoi(last);
						}
					}
				}

			}
		}
	}
	file.close();


	for (const auto& texture : vTextures)
	{
		size_t index;
		game::int2 dimensions;

		// Create ID2D1Bitmap
		Graphics::CreateTextureFromFile(texture.fileLocation, index, dimensions);

		// Create ImageData
		ImageDataList::Create(ImageData(texture.fileLocation, index, dimensions, texture.spriteCount.x, texture.spriteCount.y, texture.vAnimations));
	}
	
}
