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
	void InitScreenObjects();

	ImageData imageDatas[T_COUNT];
	WorldObject* prefabsWorld[PREFAB_W_COUNT];
	ScreenObject* prefabsScreen[PREFAB_S_COUNT];

public:
	PrefabList();

	~PrefabList();

	WorldObject* GetWorldObject(int prefabTag);
	ScreenObject* GetScreenObject(int prefabTag);
};
