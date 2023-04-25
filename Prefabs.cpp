#include "Prefabs.h"
#include "Box.h"
#include "Ball.h"
#include "Character.h"
#include "Player.h"
#include "NPC.h"

#include "SO_Button.h"

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
	imageDatas[T_BallRed] = ImageData(T_BallRed, { 32,32 });

	game::Rect* Animation_Rects{ SeperateSprites({32,32}, {8,5}) };
	game::Int2 Animation_Anims[2] = {
		{0,3},
		{4,5}
	};
	imageDatas[T_Animation] = ImageData(T_Animation, { 32,32 }, 7, Animation_Rects, 2, Animation_Anims);
	delete[] Animation_Rects;
	Animation_Rects = nullptr;

	game::Rect* Guy_Rects{ SeperateSprites({73,86}, {8,4}) };
	game::Int2 Guy_Anims[7] = {
		{0,0},
		{1,1},
		{2,2},
		{3,3},
		{8,15},
		{16,23},
		{24,31}
	};
	imageDatas[T_Guy] = ImageData(T_Guy, { 73,86 }, 32, Guy_Rects, 7, Guy_Anims);
	imageDatas[T_Guy2] = ImageData(T_Guy2, { 73,86 }, 32, Guy_Rects, 7, Guy_Anims);
	delete[] Guy_Rects;
	Guy_Rects = nullptr;

	game::Rect* Button_Rects{ SeperateSprites({128,32}, {1,2}) };
	game::Int2 Button_Anims[2] = {
		{0,0},
		{1,1}
	};
	imageDatas[T_UI_Resume] = ImageData(T_UI_Resume, { 128,32 }, 2, Button_Rects, 2, Button_Anims);
	imageDatas[T_UI_MainMenu] = ImageData(T_UI_MainMenu, { 128,32 }, 2, Button_Rects, 2, Button_Anims);
	delete[] Button_Rects;
	Button_Rects = nullptr;

	game::Rect* BG_Rects{ SeperateSprites({512,512}, {2,2}) };
	game::Int2 BG_Anims[1] = {
		{ 0,3 }
	};
	imageDatas[T_UI_BG] = ImageData(T_UI_BG, { 512,512 }, 4, BG_Rects, 1, BG_Anims);
	delete[] BG_Rects;
	BG_Rects = nullptr;

	game::Rect* Floor_Rects{ SeperateSprites({64,64}, {4,2}) };
	game::Int2 Floor_Anims[8] = {
		{0,0},
		{1,1},
		{2,2},
		{3,3},
		{4,4},
		{5,5},
		{6,6},
		{7,7}
	};
	imageDatas[T_Floor] = ImageData(T_Floor, { 64,64 }, 8, Floor_Rects, 8, Floor_Anims);
	delete[] Floor_Rects;
	Floor_Rects = nullptr;

	imageDatas[T_UI_Selection] = ImageData(T_UI_Selection, { 64,64 });

	imageDatas[T_UI_Cursor] = ImageData(T_UI_Cursor, { 8, 8 });
}

void PrefabList::InitGameObjects()
{
	prefabs[PREFAB_Player] = new Player(100.f, 3.5f, &imageDatas[T_Guy], SL_Object, { 1.f,1.f }, { 0,.6f }, .28f, true, true, 1.f, 100.f);
	prefabs[PREFAB_NPC] = new NPC(100.f, 100.f, 1.75f, &imageDatas[T_Guy2], SL_Object, { 1.f,1.f }, { 0,.6f }, .28f, true, true, 4.f, 200.f);
	prefabs[PREFAB_Mushroom] = new Ball(&imageDatas[T_Mushroom], SL_Object, { 2.f,2.f }, { .1f,2.2f }, .65f, false, true);
	prefabs[PREFAB_Floor_ConcreteA] = new WorldObject(&imageDatas[T_Floor], SL_Floor);
	prefabs[PREFAB_Floor_ConcreteB] = new WorldObject(&imageDatas[T_Floor], SL_Floor);
	prefabs[PREFAB_Floor_ConcreteC] = new WorldObject(&imageDatas[T_Floor], SL_Floor);
	prefabs[PREFAB_Floor_ConcreteD] = new WorldObject(&imageDatas[T_Floor], SL_Floor);
	prefabs[PREFAB_Floor_TileA] = new WorldObject(&imageDatas[T_Floor], SL_Floor);
	prefabs[PREFAB_Floor_TileB] = new WorldObject(&imageDatas[T_Floor], SL_Floor);
	prefabs[PREFAB_Floor_TileC] = new WorldObject(&imageDatas[T_Floor], SL_Floor);
	prefabs[PREFAB_Floor_TileD] = new WorldObject(&imageDatas[T_Floor], SL_Floor);


	prefabs[PREFAB_Selection] = new WorldObject(&imageDatas[T_UI_Selection], SL_UI_Mid);
	prefabs[PREFAB_Resume] = new SO_Button(&imageDatas[T_UI_Resume], SL_UI_Front);
	prefabs[PREFAB_MainMenu] = new SO_Button(&imageDatas[T_UI_MainMenu], SL_UI_Front);
	prefabs[PREFAB_Background] = new ScreenObject(&imageDatas[T_UI_BG], SL_UI_Back, { 1.3f,1.f }, { 0,0 }, 100.f);
	prefabs[PREFAB_Cursor] = new ScreenObject(&imageDatas[T_UI_Cursor], SL_UI_Front, {1.f,1.f}, {.12f,.12f});
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

GameObject* PrefabList::GetGameObject(int prefabTag)
{
	return prefabs[prefabTag];
}