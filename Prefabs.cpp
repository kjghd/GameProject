#include "Prefabs.h"
#include "Box.h"
#include "Ball.h"
#include "Character.h"


PrefabList::PrefabList()
{					
	prefabs[PREFAB_Player] = new Character();

									//		 Loc      Sprite								Collision
									//		          Layer		 Texture  Size     Offset   Size     Dynamic Block
	prefabs[PREFAB_Block_Dynamic] = new Box({ 0,0 }, SL_Object, T_Red,   { 1,1 }, { 0,0 }, { 1,1 }, true,   true );
	prefabs[PREFAB_Block_Wall]    =	new Box({ 0,0 }, SL_Object, T_Blue,  { 1,1 }, { 0,0 }, { 1,1 }, false,  true );
	
	prefabs[PREFAB_Block_Floor]   =	new GameObject({ 0,0 }, SL_Floor, T_Green, { 1,1 }, { 0,0 });

	
								  //	   Loc      Sprite	    								   Collision
								  //		        Layer      Texture     Size          Offset    Size Dynamic Block
	prefabs[PREFAB_BallStatic]  = new Ball({ 0,0 }, SL_Object, T_BallBlue, { 1,1      }, { 0,0 },  .5,  false,  true);
	prefabs[PREFAB_BallDynamic] = new Ball({ 0,0 }, SL_Object, T_BallRed,  { 1,1      }, { 0,0 },  .5,  true,   true);
	prefabs[PREFAB_Mushroom]    = new Ball({ 0,0 }, SL_Object, T_Mushroom, { 1.5,1.92 }, { 0,.7 }, .3,  false,  true);

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
