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
#include "Zombie.h"
#include "Leopad.h"
#include "Bat.h"

// Simon	
#define ID_TEX_SIMON				0

#define SIMON_BBOX_WIDTH			30
#define SIMON_BBOX_HEIGHT			62

#define SIMON_WALKING_SPEED			0.13f
#define SIMON_JUMP_SPEED_Y			0.5f
#define SIMON_GRAVITY				0.002f
#define SIMON_GRAVITY_LOWER			0.001f

#define IDLE		0
#define WALK		1
#define JUMP		2
#define DUCK		3
#define ASCEND		4
#define DESCEND		5
#define HURT		6
#define STANDING	7
#define DUCKING		8
#define ASCENDING	9
#define DESCENDING	10
#define	UPGRADE		11
#define HIT			12

#define SIMON_STAIR_SPEED_X			0.08f
#define SIMON_STAIR_SPEED_Y			0.08f
#define SIMON_DEFLECT_SPEED_X		0.13f
#define SIMON_DEFLECT_SPEED_Y		0.3f


#define SIMON_UNTOUCHABLE_TIME 3000

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
	bool isHitSubWeapons = false;
	bool isStandOnStair = false;
	bool isMovingUp = false;
	bool isMovingDown = false;
	int stairDirection = 0; // 1: trái dưới - phải trên, -1: trái trên - phải dưới

	LPGAMEOBJECT stairCollided = nullptr;

	bool IsTouchGround() { return vy == 0; }
	
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
	void CheckCollisionWithEnemyActiveArea(vector<LPGAMEOBJECT>* listEnemy);

	bool IsGotChainItem() { return isGotChainItem; }
	void SetGotChainItem(bool x) { isGotChainItem = x; }

	bool IsHitSubWeapons() { return isHitSubWeapons; }
	void SetHitSubWeapons(bool x) { isHitSubWeapons = x; }

	bool IsStandOnStair() { return this->isStandOnStair; }
	void SetStandOnStair(bool x) { this->isStandOnStair = x; }

	bool IsMovingUp() { return this->isMovingUp; }
	bool IsMovingDown() { return this->isMovingDown; }
	int GetStairDirection() { return this->stairDirection; }
	LPGAMEOBJECT GetStairCollided() { return this->stairCollided; }
	bool CheckCollisionWithStair(vector<LPGAMEOBJECT>* listStair);
	void PositionCorrection(int prevState = -1);
	void StandOnStair();

	
};

