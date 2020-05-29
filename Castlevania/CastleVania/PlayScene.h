#pragma once
#include "Game.h"
#include "Textures.h"
#include "Scene.h"
#include "GameObject.h"
#include "Simon.h"
#include "Torch.h"
#include "Portal.h"
#include "TileMap.h"
#include "Weapon.h"
#include "Items.h"
#include"Ground.h"
#include"Stair.h"
#include "Zombie.h"
#include "Leopad.h"
#include "Bat.h"

class PlayScene : public Scene
{
protected:
	Whip* whip;
	Weapon* weapon;
	TileMaps* tilemaps = TileMaps::GetInstance();
	Simon* player;					// A play scene has to have player, right? 

	Items* items;

	Bat* bat = new Bat();

	vector<LPGAMEOBJECT> objects;
	vector<LPGAMEOBJECT> listGrounds;
	vector<LPGAMEOBJECT> listTorchs;
	vector<LPGAMEOBJECT> listItems;
	vector<LPGAMEOBJECT> listStairs;
	vector<LPGAMEOBJECT> listZombies;
	vector<LPGAMEOBJECT> listLeopads;
	vector<LPGAMEOBJECT> listBats;
	vector<LPGAMEOBJECT> listPortals;

	void _ParseSection_TEXTURES(string line);
	void _ParseSection_SPRITES(string line);
	void _ParseSection_ANIMATIONS(string line);
	void _ParseSection_ANIMATION_SETS(string line);
	void _ParseSection_OBJECTS(string line);
	void _ParseSection_TILEMAP(string line);

	
public:
	PlayScene(int id, LPCWSTR filePath);

	virtual void Load();
	virtual void Update(DWORD dt);
	virtual void Render();
	virtual void Unload();	

	Simon* GetPlayer() { return player; }
	Weapon* GetWeapon() { return this->weapon; }

	void SetDropItems(LPGAMEOBJECT object);
	void SetInactivation();
	void UpdateCameraPosition();
	vector<LPGAMEOBJECT>* GetListStairs() { return &(this->listStairs); }


};


class PlaySceneKeyHandler : public KeyHandler
{
public:
	virtual void KeyState(BYTE* states);
	virtual void OnKeyDown(int KeyCode);
	virtual void OnKeyUp(int KeyCode);
	PlaySceneKeyHandler(Scene* s) :KeyHandler(s) {};



	void Simon_Jump();
	void Simon_Hit();
	void Simon_Hit_Weapon();

	void Simon_Stair_Down();
	void Simon_Stair_Up();
	bool Simon_Stair_Stand();

	bool StairCollisionsDetection();
};
