#include "Prefabs.h"
#include "Box.h"
#include "Ball.h"
#include "Character.h"
#include "Player.h"


void PrefabList::InitImageData()
{
	//game::Rect Error_Rect{ 0,0,15,15 };
	//game::Int2 Error_Anim{ 0,0 };
	//imageDatas[T_Error] = ImageData(T_Error, { 16,16 }, 1, &Error_Rect, 1, &Error_Anim);

	game::Rect Wallace_Rect{ 0,0,90,211 };
	game::Int2 Wallace_Anim{ 0,0 };
	imageDatas[T_Wallace] = ImageData(T_Wallace, { 91,212 }, 1, &Wallace_Rect, 1, &Wallace_Anim);

	game::Rect Mushroom_Rect{ 0,0,140,182 };
	game::Int2 Mushroom_Anim{ 0,0 };
	imageDatas[T_Mushroom] = ImageData(T_Mushroom, { 141,183 }, 1, &Mushroom_Rect, 1, &Mushroom_Anim);

	game::Rect Animation_Rects[7] = {
		{ 0, 0,  31, 31},
		{32, 0,  63, 31},
		{64, 0,  95, 31},
		{96, 0, 127, 31},

		{ 0, 32, 31, 63},
		{32, 32, 63, 63},
		{64, 32, 95, 63}
	};
	game::Int2 Animation_Anims[2] = {
		{0,3},
		{4,5}
	};
	imageDatas[T_Animation] = ImageData(T_Animation, { 32,32 }, 7, Animation_Rects, 2, Animation_Anims);

	game::Rect BallRed_Rect{ 0,0,31,31 };
	game::Int2 BallRed_Anims{ 0,0 };
	imageDatas[T_BallRed] = ImageData(T_BallRed, { 32,32 }, 1, &BallRed_Rect, 1, &BallRed_Anims);

}

void PrefabList::InitGameObjects()
{
	prefabs[PREFAB_Player] = new Player(3, &imageDatas[T_Wallace], 1, SL_Object, {1,1}, {0,.7}, .3, true, true);
	prefabs[PREFAB_BallDynamic] = new Ball(&imageDatas[T_BallRed], 1, SL_Object, {3,3}, {0,0}, .5, true, true);
	prefabs[PREFAB_Mushroom] = new Ball(&imageDatas[T_Mushroom], 1, SL_Object, {1,1}, {0,.7}, .25, false, true);
	prefabs[PREFAB_Animation] = new GameObject(&imageDatas[T_Animation], 1, SL_Floor, { 3,3 }, { 0,0 });
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
