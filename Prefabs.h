#pragma once
#include "GameObject.h"
#include "PrefabTags.h"


class PrefabList
{
	static GameObject* prefabs[PREFAB_COUNT];

public:
	static void Initialise();
	~PrefabList();
	static GameObject* Get(int prefabTag);
};
