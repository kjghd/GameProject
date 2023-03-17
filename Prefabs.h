#pragma once

#include "GameObject.h"
#include "ImageData.h"
#include "PrefabTags.h"
#include "Textures.h"

#include <vector>


class PrefabList
{
	void InitImageData();
	void InitGameObjects();

	GameObject* prefabs[PREFAB_COUNT];
	ImageData imageDatas[T_COUNT];

public:
	PrefabList();

	~PrefabList();

	GameObject* Get(int prefabTag);
};

static PrefabList prefabList;
