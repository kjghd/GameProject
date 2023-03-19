#include "Prefabs.h"
#include "Box.h"
#include "Ball.h"
#include "Character.h"
#include "Player.h"

game::Rect* SeperateSprites(game::Float2 px, game::Int2 size)
{
	game::Rect *pRects = new game::Rect[size.x * size.y];

	for (size_t y = 0; y < size.y; y++)
		for (size_t x = 0; x < size.x; x++)
			pRects[y * size.x + x] = {
				(float)px.x * x,
				(float)px.y * y,
				(float)px.x * (x + 1),
				(float)px.y * (y + 1)
			};

	return pRects;
}

void PrefabList::InitImageData()
{
	imageDatas[T_Wallace] = ImageData(T_Wallace, { 91,212 });
	
	imageDatas[T_Mushroom] = ImageData(T_Mushroom, { 141,183 });

	game::Rect* Animation_Rects{ SeperateSprites({73,86}, {8,5}) };
	game::Int2 Animation_Anims[2] = {
		{0,3},
		{4,5}
	};
	imageDatas[T_Animation] = ImageData(T_Animation, { 32,32 }, 7, Animation_Rects, 2, Animation_Anims);
	delete[] Animation_Rects;
	Animation_Rects = nullptr;

	game::Rect* Guy_Rects{ SeperateSprites({73,86}, {8,5}) };
	game::Int2 Guy_Anims[6] = {
		{0,0},
		{1,1},
		{8,15},
		{16,23},
		{24,31},
		{32,39}
	};
	imageDatas[T_Guy] = ImageData(T_Guy, { 73,86 }, 40, Guy_Rects, 6, Guy_Anims);
	delete[] Guy_Rects;
	Guy_Rects = nullptr;

	imageDatas[T_BallRed] = ImageData(T_BallRed, { 32,32 });

}

void PrefabList::InitGameObjects()
{
	prefabs[PREFAB_Player] = new Player(3.5, &imageDatas[T_Guy], 110, SL_Object, {.6,.6}, {0,.6}, .28, true, true);
	prefabs[PREFAB_BallDynamic] = new Ball(&imageDatas[T_BallRed], SL_Object, {1,1}, {0,0}, .5, true, true);
	prefabs[PREFAB_Mushroom] = new Ball(&imageDatas[T_Mushroom], SL_Object, {1,1}, {.1,2.2}, .65, false, true);
	prefabs[PREFAB_Animation] = new GameObject(&imageDatas[T_Animation], 500, SL_Floor, { 1,1 }, { 0,0 });
}

PrefabList::PrefabList()
	:
	imageDatas()
{					
	InitImageData();
	InitGameObjects();
}

PrefabList::~PrefabList()
{
	for (size_t i{ 0 }; i < PREFAB_COUNT; ++i)
	{
		delete prefabs[i];
		prefabs[i] = nullptr;
	}
}

GameObject* PrefabList::Get(int prefabTag)
{
	return prefabs[prefabTag];
}
