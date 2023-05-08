#include "ImageDataList.h"
#include "DataTypes.h"
#include <vector>


//ImageData ImageDataList::list[T_COUNT];

//game::rect* SeperateSprites(game::float2 px, game::int2 size)
//{
//	game::rect* pRects = new game::rect[size.x * size.y];
//
//	for (size_t y = 0; y < size.y; y++)
//		for (size_t x = 0; x < size.x; x++)
//			pRects[y * size.x + x] = {
//				(float)px.x * x,
//				(float)px.y * y,
//				(float)px.x * (x + 1),
//				(float)px.y * (y + 1)
//		};
//
//	return pRects;
//}

//void ImageDataList::Initialise()
//{
//	list[T_Wallace] = ImageData(T_Wallace, { 91,212 });
//	list[T_Mushroom] = ImageData(T_Mushroom, { 141,183 });
//	list[T_BallRed] = ImageData(T_BallRed, { 32,32 });
//
//	game::rect* Animation_Rects{ SeperateSprites({32,32}, {8,5}) };
//	game::int2 Animation_Anims[2] = {	{0,3},	{4,5}
//;
//	list[T_Animation] = ImageData(T_Animation, { 32,32 }, 7, Animation_Rects, 2, Animation_Anims);
//	delete[] Animation_Rects;
//	Animation_Rects = nullptr;
//
//	game::rect* Guy_Rects{ SeperateSprites({73,86}, {8,4}) };
//	game::int2 Guy_Anims[7] = {	{0,0},	{1,1},	{2,2},	{3,3},	{8,15},	{16,23},	{24,31}
//;
//	list[T_Guy] = ImageData(T_Guy, { 73,86 }, 32, Guy_Rects, 7, Guy_Anims);
//	list[T_Guy2] = ImageData(T_Guy2, { 73,86 }, 32, Guy_Rects, 7, Guy_Anims);
//	delete[] Guy_Rects;
//	Guy_Rects = nullptr;
//
//	game::rect* Button_Rects{ SeperateSprites({128,32}, {1,2}) };
//	game::int2 Button_Anims[2] = {	{0,0},	{1,1}
//;
//	list[T_UI_Resume] = ImageData(T_UI_Resume, { 128,32 }, 2, Button_Rects, 2, Button_Anims);
//	list[T_UI_MainMenu] = ImageData(T_UI_MainMenu, { 128,32 }, 2, Button_Rects, 2, Button_Anims);
//	delete[] Button_Rects;
//	Button_Rects = nullptr;
//
//	game::rect* BG_Rects{ SeperateSprites({512,512}, {2,2}) };
//	game::int2 BG_Anims[1] = {
//		{ 0,3 }
//	};
//	list[T_UI_BG] = ImageData(T_UI_BG, { 512,512 }, 4, BG_Rects, 1, BG_Anims);
//	delete[] BG_Rects;
//	BG_Rects = nullptr;
//
//	game::rect* Floor_Rects{ SeperateSprites({64,64}, {4,2}) };
//	game::int2 Floor_Anims[8] = {	{0,0},	{1,1},	{2,2},	{3,3},	{4,4},	{5,5},	{6,6},	{7,7}
//;
//	list[T_Floor] = ImageData(T_Floor, { 64,64 }, 8, Floor_Rects, 8, Floor_Anims);
//	delete[] Floor_Rects;
//	Floor_Rects = nullptr;
//
//	list[T_UI_Selection] = ImageData(T_UI_Selection, { 64,64 });
//
//	list[T_UI_Cursor] = ImageData(T_UI_Cursor, { 8, 8 });
//
//	game::rect* Blink_Rects{ SeperateSprites({4,4}, {2,1}) };
//	game::int2 Blink_Anims[1] = {
//		{ 0,1 }
//	};
//	list[T_UI_CursorBlinking] = ImageData(T_UI_CursorBlinking, { 8, 8 }, 2, Blink_Rects, 1, Blink_Anims);
//	delete Blink_Rects;
//	Blink_Rects = nullptr;
//}


std::vector<ImageData> ImageDataList::vList;

void ImageDataList::Create(ImageData imageData)
{
	vList.push_back(imageData);
}

ImageData* ImageDataList::Get(std::string path)
{
	for (auto& imageData : vList)
		if (imageData.MatchPath(path))
			return &imageData;
}

