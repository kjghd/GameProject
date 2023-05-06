#pragma once

#include "GameObject.h"
#include "PrefabTags.h"

#include <vector>


class PrefabList
{
	static GameObject* prefabs[PREFAB_COUNT];

public:
	static void Initialise();
	~PrefabList();
	static GameObject* Get(int prefabTag);
};
