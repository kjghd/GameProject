#pragma once
#include <string>
#include <fstream>

namespace game
{
	struct int2
	{
		int x = 0, y = 0;
	};

	struct float2
	{
		float x = 0, y = 0;

		float2 operator *(float f)
		{
			return { x * f, y * f };
		}

		float2 operator /(float f)
		{
			return { x / f, y / f };
		}
		
		float2 operator +(float f)
		{
			return { x + f, y + f };
		}
		float2 operator +(float2 ff)
		{
			return { x + ff.x, y + ff.y };
		}
		
		float2 operator -()
		{
			return { -x, -y };
		}
		float2 operator -(float f)
		{
			return { x - f, y - f };
		}
		float2 operator -(float2 ff)
		{
			return { x - ff.x, y - ff.y };
		}

		bool operator ==(float2 ff) { return x == ff.x && y == ff.y ? true : false; }

		void operator +=(float2 ff) { x += ff.x; y += ff.y; };
		void operator -=(float2 ff) { x -= ff.x; y -= ff.y; };
	};

	struct rect
	{
		float l, t, r, b;
	};

	//template<class T>
	//std::string DataToString(T data)
	//{
	//	std::string str;
	//	char bytes[sizeof(T)]{};
	//
	//	char* address{ (char*)&data };
	//
	//	for (size_t i{ 0 }; i < sizeof(T); ++i)
	//	{
	//		str.push_back(*(address + i));
	//	}
	//
	//	return str;
	//}
	//
	//template<class T>
	//T GetBytes(std::ifstream& file)
	//{
	//	char buff[sizeof(T)]{};
	//	file.read(buff, sizeof(T));
	//
	//	return (T)buff;
	//}

}
