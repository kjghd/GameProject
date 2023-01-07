#include "Prefabs.h"
#include "Tile.h"
#include "Ball.h"


PrefabList::PrefabList()			//		 Loc      Sprite					    Collision
{									//		          Layer		  Texture  Size     Offset   Size     Dynamic Block
	prefabs[PREFAB_Block_Dynamic] = new Tile({ 0,0 }, SL_Dynamic, T_Red,   { 1,1 }, { 0,0 }, { 1,1 }, true,   true );
	prefabs[PREFAB_Block_Wall]    =	new Tile({ 0,0 }, SL_Wall,	  T_Blue,  { 1,1 }, { 0,0 }, { 1,1 }, false,  true );
	prefabs[PREFAB_Block_Floor]   =	new Tile({ 0,0 }, SL_Floor,   T_Green, { 1,1 }, { 0,0 }, { 1,1 }, false,  false);
	
								  //	   Loc      Sprite					     Collision
								  //		        Layer    Texture     Size     Offset   Size Dynamic Block
	prefabs[PREFAB_BallStatic]  = new Ball({ 0,0 }, SL_Wall, T_BallBlue, { 1,1 }, { 0,0 }, .5,  false,  true);
	prefabs[PREFAB_BallDynamic] = new Ball({ 0,0 }, SL_Wall, T_BallRed, { 1,1 }, { 0,0 }, .5,  true,   true);
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
