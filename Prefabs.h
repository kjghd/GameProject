#pragma once

#include "GameObject.h"
#include "PrefabTags.h"

#include <vector>


class PrefabList
{
	GameObject* prefabs[PREFAB_COUNT];

public:
	PrefabList();

	~PrefabList();

	GameObject* Get(int prefabTag);
};

static PrefabList prefabList;
