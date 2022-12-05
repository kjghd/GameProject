#include "Prefabs.h"

PrefabList::PrefabList()
{
	prefabs[PREFAB_Block_Brick_01] = Tile(T_Bricks_01, { 0,0 }, true);
	prefabs[PREFAB_Block_Pavement_01] = Tile(T_Pavement_01, { 0,0 }, false);
}

const Tile* PrefabList::Get(int prefabTag)
{
	return &prefabs[prefabTag];
}
