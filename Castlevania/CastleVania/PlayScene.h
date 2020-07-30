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
#include "Bone.h"
#include "Grid.h"



class PlayScene : public Scene
{
protected:
	Whip* whip;
	Weapon* weapon;
	
	Simon* player;					// A play scene has to have player, right? 

	Boss* boss;

	Grid* grid;
	Unit* unit;

	TileMaps* tilemaps = TileMaps::GetInstance();

	vector<Unit*> listUnits;
	vector<LPGAMEOBJECT> listObjects;
	vector<LPGAMEOBJECT> listItems;
	vector<LPGAMEOBJECT> listStairs;
	vector<LPGAMEOBJECT> listStaticObjectsToRender;
	vector<LPGAMEOBJECT> listMovingObjectsToRender;
	vector<Weapon*> weaponlist;	// dùng mảng để sử dụng double, triple shot

	BlockMove* block;

	Portal* portal;

	bool isSetSimonAutoWalk = false;

	bool isUsingStopWatch = false;
	int stopWatchCounter = 0;

	bool isSimonDead = false;			// dừng update simon khi ở trạng thái dead
	int simonDeadTimeCounter = 0;

	bool isDoubleShot = false;
	int doubleShotTimeCounter = 0;

	bool isTripleShot = false;
	int tripleShotTimeCounter = 0;

	bool isBossFighting = false;


	void _ParseSection_TEXTURES(string line);
	void _ParseSection_SPRITES(string line);
	void _ParseSection_ANIMATIONS(string line);
	void _ParseSection_ANIMATION_SETS(string line);
	void _ParseSection_OBJECTS(string line);
	void _ParseSection_TILEMAP(string line);
	void _ParseSection_GRID(string line);
	
public:
	PlayScene(int id, LPCWSTR filePath);

	virtual void Load();
	virtual void Update(DWORD dt);
	virtual void Render();
	virtual void Unload();	

	Simon* GetPlayer() { return player; }
	Whip* GetWhip() { return whip; }
	Weapon* GetWeapon() { return this->weapon; }
	Boss* GetBoss() { return this->boss; }

	void SetDropItems();
	int GetRandomItem();

	void SetInactivation();
	bool IsInViewport(LPGAMEOBJECT object);
	void UpdateCameraPosition();
	vector<LPGAMEOBJECT>* GetListStairs() { return &(this->listStairs); }

	void UpdateTimeCounter();

	bool isGameReset = false;
	bool isGameOver = false;
	bool isGamePause = false;

	void Simon_Update(DWORD dt);
	void Whip_Update(DWORD dt);
	void Weapon_Update(DWORD dt, int index);

	// Nhận các đối tượng cần xét
	void GetColliableObjects(LPGAMEOBJECT curObj, vector<LPGAMEOBJECT>& coObjects);
	void GetObjectFromGrid();
	void UpdateGrid();

	void SetEnemiesSpawnPositon();				// Tạo vị trí xuất hiện enemies
	vector<Weapon*>* GetWeaponList() { return &weaponlist; }

	
	// Item Effect
	bool IsUsingStopWatch() { return isUsingStopWatch; }
	bool IsDoubleShot() { return isDoubleShot; }
	bool IsTripleShot() { return isTripleShot; }


	void StartStopWatch() { isUsingStopWatch = true; stopWatchCounter = GetTickCount(); }
	void DoubleShotEffect();
	void TripleShotEffect();

	void ResetGame();							// Reset lại trạng thái của game (map, simon...) sau khi simon chết

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

	bool AnimationDelay();
	bool CanProcessKeyboard();

	bool StairCollisionsDetection();
};
