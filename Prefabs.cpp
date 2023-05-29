#include "Prefabs.h"

#include "Box.h"
#include "Ball.h"
#include "Character.h"
#include "Player.h"
#include "NPC.h"

#include "SO_Button.h"

#include "ImageDataList.h"


GameObject* PrefabList::prefabs[PREFAB_COUNT];

void PrefabList::Initialise()
{
	prefabs[PREFAB_Player] = new Player(100.f, 3.5f, ImageDataList::Get("guy_sheet.png"), SL_Object, {1.f,1.f}, {0,.6f}, .28f, true, true, 1.f, 100.f);
	prefabs[PREFAB_NPC] = new NPC(100.f, 100.f, 1.75f, ImageDataList::Get("guy_sheet2.png"), SL_Object, { 1.f,1.f }, { 0,.6f }, .28f, true, true, 4.f, 200.f);
	prefabs[PREFAB_Mushroom] = new Ball(ImageDataList::Get("Mushroom.png"), SL_Object, { 2.f,2.f }, { .1f,2.2f }, .65f, false, true);
	prefabs[PREFAB_Floor_ConcreteA] = new WorldObject(ImageDataList::Get("shopping_floor.png"), SL_Floor);
	//prefabs[PREFAB_Floor_ConcreteB] = new WorldObject(ImageDataList::Get("shopping_floor.png"), SL_Floor);
	//prefabs[PREFAB_Floor_ConcreteC] = new WorldObject(ImageDataList::Get("shopping_floor.png"), SL_Floor);
	//prefabs[PREFAB_Floor_ConcreteD] = new WorldObject(ImageDataList::Get("shopping_floor.png"), SL_Floor);

	prefabs[PREFAB_Selection] = new WorldObject(ImageDataList::Get("TileSelection.png"), SL_UI_Mid);
	prefabs[PREFAB_Resume] = new SO_Button(ImageDataList::Get("Resume.png"), SL_UI_Mid);
	prefabs[PREFAB_MainMenu] = new SO_Button(ImageDataList::Get("MainMenu.png"), SL_UI_Mid);
	prefabs[PREFAB_Save] = new SO_Button(ImageDataList::Get("Save.png"), SL_UI_Mid);
	prefabs[PREFAB_Load] = new SO_Button(ImageDataList::Get("Load.png"), SL_UI_Mid);
	prefabs[PREFAB_New] = new SO_Button(ImageDataList::Get("New.png"), SL_UI_Mid);
	prefabs[PREFAB_Background] = new ScreenObject(ImageDataList::Get("BG.png"), SL_UI_Back, { 1.3f,1.f }, { 0,0 }, 100.f);
	prefabs[PREFAB_Cursor] = new ScreenObject(ImageDataList::Get("CursorBlinking.png"), SL_UI_Front, {1.f,1.f}, {0,0}, 400.f);
}

PrefabList::~PrefabList()
{
	for (size_t i{ 0 }; i < PREFAB_COUNT; ++i)
	{
		delete prefabs[i];
		prefabs[i] = nullptr;
	}
}

GameObject* PrefabList::Get(int prefabTag)
{
	return prefabs[prefabTag];
}
