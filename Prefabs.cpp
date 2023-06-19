#include "Prefabs.h"

#include "Box.h"
#include "Ball.h"
#include "Character.h"
#include "Player.h"
#include "NPC.h"

#include "SO_Button.h"
#include "SO_TextBox.h"

#include "ImageDataList.h"


GameObject* PrefabList::prefabs[PREFAB_COUNT];

void PrefabList::Initialise()
{
	prefabs[PREFAB_Player] = new Player(100.f, 2.f, ImageDataList::Get("human.png"), SL_Object, { 1.f,1.f }, { 0,.6f }, .28f, true, true, 1.f, 80.f);
	prefabs[PREFAB_NPC] = new NPC(10.f, 100.f, 2.f, ImageDataList::Get("human.png"), SL_Object, { 1.f,1.f }, { 0,.6f }, .28f, true, true, 1.f, 80.f);
	prefabs[PREFAB_Mushroom] = new Ball(ImageDataList::Get("Mushroom.png"), SL_Object, { 2.f,2.f }, { .1f,2.2f }, .65f, false, true);
	prefabs[PREFAB_Field] = new WorldObject(ImageDataList::Get("field.png"), SL_Floor);
	
	prefabs[PREFAB_Selection] = new WorldObject(ImageDataList::Get("TileSelection.png"), SL_UI_Mid);
	prefabs[PREFAB_Resume] = new SO_Button("Resume");
	prefabs[PREFAB_MainMenu] = new SO_Button("Main Menu");
	prefabs[PREFAB_Save] = new SO_Button("Save");
	prefabs[PREFAB_Load] = new SO_Button("Load");
	prefabs[PREFAB_New] = new SO_Button("New");
	prefabs[PREFAB_Background] = new ScreenObject(ImageDataList::Get("BG.png"), SL_UI_Back, { 1.3f,1.f }, { 0,0 }, 100.f);
	prefabs[PREFAB_Cursor] = new ScreenObject(ImageDataList::Get("CursorBlinking.png"), SL_UI_Front, {1.f,1.f}, {0,0}, 400.f);
	prefabs[PREFAB_TextBox] = new SO_TextBox("This is text\nI've written! :3", .5f, 2, false, ImageDataList::Get("BG.png"), SL_UI_Mid);
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
