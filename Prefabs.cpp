#include "Prefabs.h"

PrefabList::PrefabList()
{
	prefabs[PREFAB_Block_Brick_01] = Tile(T_Bricks_01, { 0,0 }, false, true);
	prefabs[PREFAB_Block_Pavement_01] = Tile(T_Pavement_01, { 0,0 }, false, false);

	prefabs[PREFAB_Block_Dynamic] = Tile(T_Red, { 0,0 }, true, true);
	prefabs[PREFAB_Block_Wall] = Tile(T_Blue, { 0,0 }, false, true);
	prefabs[PREFAB_Block_Floor] = Tile(T_Green, { 0,0 }, false, false);

}

const Tile* PrefabList::Get(int prefabTag)
{
	return &prefabs[prefabTag];
}
