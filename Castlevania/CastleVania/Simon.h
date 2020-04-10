#pragma once

#include "GameObject.h"
#include "Textures.h"
#include "Define.h"
#include "Whip.h"

// Simon	
#define ID_TEX_SIMON				0

#define SIMON_BBOX_WIDTH			32
#define SIMON_BBOX_HEIGHT			32

#define SIMON_WALKING_SPEED			0.1f
#define SIMON_JUMP_SPEED_Y			0.5f
#define SIMON_GRAVITY				0.03f

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


#define SIMON_UNTOUCHABLE_TIME 3000

using namespace std;

class Simon : public GameObject
{
	bool isStand = true;

	Whip* whip;

	int untouchable;
	DWORD untouchable_start;
public:
	Simon();

	
	void Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects = NULL);
	void Render();
	void SetState(int state);
	void StartUntouchable() { untouchable = 1; untouchable_start = GetTickCount(); }
	void SetWhipAnimationSet(LPANIMATION_SET ani_set);

	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);

	bool IsStand();
};

