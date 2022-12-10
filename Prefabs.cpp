#include "Prefabs.h"


PrefabList::PrefabList()
{									//   Loc      Tex      Tex Size Tex Off  Dyn    Block
	prefabs[PREFAB_Block_Dynamic] = Tile({ 0,0 }, T_Red,   { 1,1 }, { 0,0 }, true,  true );
	prefabs[PREFAB_Block_Wall]    =	Tile({ 0,0 }, T_Blue,  { 1,1 }, { 0,0 }, false, true );
	prefabs[PREFAB_Block_Floor]   =	Tile({ 0,0 }, T_Green, { 1,1 }, { 0,0 }, false, false);
}

const Tile* PrefabList::Get(int prefabTag)
{
	return &prefabs[prefabTag];
}
