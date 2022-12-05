#pragma once
#include "WorldObject.h"
#include <vector>
#include "PrefabTags.h"

class PrefabList
{
	Tile prefabs[PREFAB_COUNT];

public:
	PrefabList();

	const Tile* Get(int prefabTag);
};

static PrefabList prefabList;