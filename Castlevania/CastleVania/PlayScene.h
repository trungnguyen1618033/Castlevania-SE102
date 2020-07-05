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
#include"BlockMove.h"
#include "Bat.h"
#include "Knight.h"
#include "Ghost.h"
#include "HunchBack.h"
#include "Raven.h"
#include "Skeleton.h"



class PlayScene : public Scene
{
protected:
	Whip* whip;
	Weapon* weapon;
	TileMaps* tilemaps = TileMaps::GetInstance();
	Simon* player;					// A play scene has to have player, right? 

	Items* items;

	Boss* boss;

	vector<LPGAMEOBJECT> listObjects;
	vector<LPGAMEOBJECT> listItems;
	vector<LPGAMEOBJECT> listStairs;
	vector<LPGAMEOBJECT> listStaticObjectsToRender;
	vector<LPGAMEOBJECT> listMovingObjectsToRender;
	vector<Weapon*> weaponlist;	// dùng mảng để sử dụng double, triple shot

	BlockMove* block;

	Portal* portal;


	bool isSetSimonAutoWalk = false;
	bool isBossFighting = false;

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

	void SetDropItems();
	void SetInactivation();
	bool IsInViewport(LPGAMEOBJECT object);
	void UpdateCameraPosition();
	vector<LPGAMEOBJECT>* GetListStairs() { return &(this->listStairs); }

	void Simon_Update(DWORD dt);
	void Whip_Update(DWORD dt);
	void Weapon_Update(DWORD dt, int index);

	// Nhận các đối tượng cần xét
	void GetColliableObjects(LPGAMEOBJECT curObj, vector<LPGAMEOBJECT>& coObjects);
	void SetEnemiesSpawnPositon();				// Tạo vị trí xuất hiện enemies
	vector<Weapon*>* GetWeaponList() { return &weaponlist; }


};


class PlaySceneKeyHandler : public KeyHandler
{
	bool isNeedToWaitingAnimation = true;

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
