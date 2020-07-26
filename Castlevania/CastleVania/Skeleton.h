#pragma once
#include "Enemy.h"
#include "Ground.h"
#define SKELETON_ACTIVE			0
#define SKELETON_DESTROYED		1
#define SKELETON_INACTIVE		4
#define SKELETON_JUMP			2
#define SKELETON_HIT			3


class Skeleton : public Enemy
{
	bool isJumping;
public:
	Skeleton();

	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL, bool stopMoving = false);
	virtual void Render();
	virtual void SetState(int state);

	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void GetActiveBoundingBox(float& left, float& top, float& right, float& bottom);

	virtual void LoseHP(int x);
};

