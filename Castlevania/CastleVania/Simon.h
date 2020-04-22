#pragma once

#include "GameObject.h"
#include "Textures.h"
#include "Define.h"
#include "Whip.h"
#include "Torch.h"
#include "Knife.h"
#include "Items.h"
#include "Heart.h"
#include "MoneyBag.h"
#include"Ground.h"

// Simon	
#define ID_TEX_SIMON				0

#define SIMON_BBOX_WIDTH			18
#define SIMON_BBOX_HEIGHT			32

#define SIMON_WALKING_SPEED			0.1f
#define SIMON_JUMP_SPEED_Y			0.5f
#define SIMON_GRAVITY				0.002f
#define SIMON_GRAVITY_LOWER			0.001f

#define IDLE		0
#define WALK		1
#define DUCK		2
#define JUMP		3
#define ASCEND		4
#define DESCEND		5
#define HURT		6
#define DEATH		7
#define STANDING	8
#define DUCKING		9
#define ASCENDING	10
#define DESCENDING	11
#define	THROW		12


#define SIMON_UNTOUCHABLE_TIME 3000

using namespace std;

class Simon : public GameObject
{
	Whip* whip;
	Knife* knife;

	int untouchable;
	DWORD untouchable_start;
public:
	bool isStand = true;
	bool hasKnife = true;
	Simon();
	bool IsTouchGround() { return vy == 0; }
	
	void Update(DWORD dt, vector<LPGAMEOBJECT>* Objects = NULL, vector<LPGAMEOBJECT>* coObjects = NULL);
	void Render();
	void SetState(int state);
	void StartUntouchable() { untouchable = 1; untouchable_start = GetTickCount(); }
	void SetWhipAnimationSet(LPANIMATION_SET ani_set);

	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);

	Knife* GetKnife() { return this->knife; }

};

