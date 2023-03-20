#include "Prefabs.h"
#include "Box.h"
#include "Ball.h"
#include "Character.h"
#include "Player.h"
#include "NPC.h"

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

	game::Rect* Animation_Rects{ SeperateSprites({32,32}, {8,5}) };
	game::Int2 Animation_Anims[2] = {
		{0,3},
		{4,5}
	};
	imageDatas[T_Animation] = ImageData(T_Animation, { 32,32 }, 7, Animation_Rects, 2, Animation_Anims);
	delete[] Animation_Rects;
	Animation_Rects = nullptr;

	game::Rect* Guy_Rects{ SeperateSprites({73,86}, {8,5}) };
	game::Int2 Guy_Anims[7] = {
		{0,0},
		{1,1},
		{2,2},
		{8,15},
		{16,23},
		{24,31},
		{32,39}
	};
	imageDatas[T_Guy] = ImageData(T_Guy, { 73,86 }, 40, Guy_Rects, 7, Guy_Anims);
	imageDatas[T_Guy2] = ImageData(T_Guy2, { 73,86 }, 40, Guy_Rects, 7, Guy_Anims);
	delete[] Guy_Rects;
	Guy_Rects = nullptr;

	imageDatas[T_BallRed] = ImageData(T_BallRed, { 32,32 });

}

void PrefabList::InitGameObjects()
{
	prefabs[PREFAB_Player] = new Player(100.f, 3.5f, &imageDatas[T_Guy], SL_Object, {.6f,.6f}, {0,.6f}, .28f, true, true, 1.f, 100.f);
	prefabs[PREFAB_NPC] = new NPC(100.f, 100.f, 1.75f, &imageDatas[T_Guy2], SL_Object, {.6f,.6f}, {0,.6f}, .28f, true, true, 4.f, 200.f);
	prefabs[PREFAB_BallDynamic] = new Ball(&imageDatas[T_BallRed], SL_Object, {1.f,1.f}, {0,0}, .5f, true, true);
	prefabs[PREFAB_Mushroom] = new Ball(&imageDatas[T_Mushroom], SL_Object, {1.f,1.f}, {.1f,2.2f}, .65f, false, true);
	prefabs[PREFAB_Animation] = new GameObject(&imageDatas[T_Animation], SL_Floor, { 1.f,1.f }, { 0,0 }, 500.f);
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
