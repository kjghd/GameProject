#include "Prefabs.h"


PrefabList::PrefabList()			//            Sprite					 Collision
{									//   Loc      Layer		  Texture  Size     Offset   Size     Dynamic Block
	prefabs[PREFAB_Block_Dynamic] = Tile({ 0,0 }, SL_Dynamic, T_Red,   { 1,1 }, { 0,0 }, { 1,1 }, true,   true );
	prefabs[PREFAB_Block_Wall]    =	Tile({ 0,0 }, SL_Wall,	  T_Blue,  { 1,1 }, { 0,0 }, { 1,1 }, false,  true );
	prefabs[PREFAB_Block_Floor]   =	Tile({ 0,0 }, SL_Floor,   T_Green, { 1,1 }, { 0,0 }, { 1,1 }, false,  false);
}

const Tile* PrefabList::Get(int prefabTag)
{
	return &prefabs[prefabTag];
}
