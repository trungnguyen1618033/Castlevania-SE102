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
#include "BlockMove.h"
#include "Knight.h"
#include "Bat.h"
#include "Ghost.h"
#include "HunchBack.h"
#include "Skeleton.h"
#include "Bone.h"
#include "Raven.h"
#include "Zombie.h"
#include "Boss.h"

using namespace std;

class Simon : public GameObject
{
	static Simon* _instance;

	int score;
	int life;
	int hp;
	int energy;
	int subWeapon;
	int stateShot;

	float autoWalkDistance = 0;		// Khoảng cách 
	int stateAfterAutoWalk = -1;	// Trạng thái sau khi auto-walk
	int nxAfterAutoWalk = 0;		// Hướng Simon sau khi auto-walk

	bool isUntouchable = false;
	DWORD untouchable_start = 0;

public:
	Simon();
	~Simon() {};

	bool isStand = true;
	bool hasWeapon = true;
	bool isGotChainItem = false;
	bool isHitWeapons = false;

	bool isFalling = false;
	bool isTouchGround = false; 
	bool isStandOnStair = false; // trạng thái đang đứng trên cầu thang 
	bool canMoveUpStair = false;	// có thể di chuyển lên cầu thang
	bool canMoveDownStair = false;	// có thể di chuyển xuống cầu thang
	bool isAutoWalk = false;		// tự động đi
	int stairDirection = 0;			// 1: trái dưới - phải trên, -1: trái trên - phải dưới

	bool isGotDoubleShotItem = false; // Double shot item
	bool isGotTripleShotItem = false; // Triple shot item

	bool isGotMagicCrystalItem = false;

	bool isDead = false;

	LPGAMEOBJECT stairCollided = nullptr;

	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL, bool stopMoving = false);
	void Render();
	void SetState(int state);
	void StartUntouchable() { isUntouchable = true; untouchable_start = GetTickCount(); }

	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);

	// Lấy thuộc tính
	bool IsStand() { return this->isStand; }
	int GetLife() { return this->life; }
	int GetScore() { return this->score; }
	int GetHP() { return this->hp; }
	int GetEnergy() { return this->energy; }
	int GetSubWeapon() { return this->subWeapon; }


	// Cập nhập thuộc tính
	void AddScore(int x) { score += x; }
	void LoseEnergy(int x) { energy -= x; }
	void LoseHP(int x);
	void SetScore(int x) { score = x; }
	void SetLife(int x) { life = x; }
	void SetHP(int x) { hp = x; }
	void SetEnergy(int x) { energy = x; }
	void SetSubWeapon(int x) { subWeapon = x; }
	


	// Kiểm tra va chạm với danh sách items
	bool CheckCollisionWithItem(vector<LPGAMEOBJECT>* listItem);

	bool IsGotChainItem() { return isGotChainItem; }
	void SetGotChainItem(bool x) { isGotChainItem = x; }

	bool IsHitWeapons() { return isHitWeapons; }
	void SetHitWeapons(bool x) { isHitWeapons = x; }

	// Kiểm tra va chạm với danh sách stairs
	bool CheckCollisionWithStair(vector<LPGAMEOBJECT>* listStair);
	bool IsStandOnStair() { return this->isStandOnStair; }
	void SetStandOnStair(bool x) { this->isStandOnStair = x; }
	int GetStairDirection() { return this->stairDirection; }
	LPGAMEOBJECT GetStairCollided() { return this->stairCollided; }

	// Simon đứng yên trên stairs
	void StandOnStair();
	// Auto-walk
	void AutoWalk(float distance, int new_state, int new_nx);
	void DoAutoWalk();

	// Xác định trạng thái đang đánh
	bool IsHit();

	// Kiểm tra va chạm với vùng hoạt động của enemy
	void CheckCollisionWithEnemyActiveArea(vector<LPGAMEOBJECT>* listObjects);

	static Simon* GetInstance();
	
};

