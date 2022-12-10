#pragma once

#include "Tile.h"
#include "PrefabTags.h"

#include <vector>


class PrefabList
{
	Tile prefabs[PREFAB_COUNT];

public:
	PrefabList();

	const Tile* Get(int prefabTag);
};

static PrefabList prefabList;