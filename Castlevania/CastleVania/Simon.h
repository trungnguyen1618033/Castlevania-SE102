#pragma once

#include "GameObject.h"
#include "Textures.h"
#include "Define.h"
#include "Whip.h"
#include "Torch.h"
#include "Weapon.h"
#include "Items.h"
#include "Ground.h"
#include "Portal.h"


using namespace std;

class Simon : public GameObject
{
	bool isUntouchable = false;
	DWORD untouchable_start = 0;

	int score;
	int life;
	int HP;
	int energy;
	int subWeapon;

public:
	Simon();
	~Simon() {};

	bool isStand = true;
	bool hasWeapon = true;
	bool isGotChainItem = false;
	bool isHitWeapons = false;

	bool isTouchGround = false; 
	bool isStandOnStair = false;
	bool isMovingUp = false;
	bool isMovingDown = false;
	int stairDirection = 0; 

	LPGAMEOBJECT stairCollided = nullptr;

	bool IsTouchGround() { return isTouchGround; }
	
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL);
	void Render();
	void SetState(int state);
	void StartUntouchable() { isUntouchable = true; untouchable_start = GetTickCount(); }

	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);

	bool IsStand() { return this->isStand; }
	int GetLife() { return this->life; }
	int GetScore() { return this->score; }
	int GetHP() { return this->HP; }
	int GetEnergy() { return this->energy; }
	int GetSubWeapon() { return this->subWeapon; }
	

	void LoseEnergy(int amount) { energy -= amount; }
	void SetSubWeapon(int x) { subWeapon = x; }
	bool CheckCollisionWithItem(vector<LPGAMEOBJECT>* listItem);

	bool IsGotChainItem() { return isGotChainItem; }
	void SetGotChainItem(bool x) { isGotChainItem = x; }

	bool IsHitWeapons() { return isHitWeapons; }
	void SetHitWeapons(bool x) { isHitWeapons = x; }

	bool IsStandOnStair() { return this->isStandOnStair; }
	void SetStandOnStair(bool x) { this->isStandOnStair = x; }

	bool IsMovingUp() { return this->isMovingUp; }
	bool IsMovingDown() { return this->isMovingDown; }
	int GetStairDirection() { return this->stairDirection; }
	LPGAMEOBJECT GetStairCollided() { return this->stairCollided; }
	bool CheckCollisionWithStair(vector<LPGAMEOBJECT>* listStair);
	//
	void StandOnStair();

	bool IsHit();

	
};

