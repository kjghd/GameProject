#pragma once

#include "WorldObject.h"
#include "ScreenObject.h"
#include "ImageData.h"
#include "PrefabTags.h"
#include "Textures.h"

#include <vector>


class PrefabList
{
	void InitImageData();
	void InitGameObjects();

	ImageData imageDatas[T_COUNT];
	GameObject* prefabs[PREFAB_COUNT];

public:
	PrefabList();

	~PrefabList();

	GameObject* GetGameObject(int prefabTag);
};
