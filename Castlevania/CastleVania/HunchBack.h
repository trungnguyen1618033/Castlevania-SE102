#pragma once
#include "Enemy.h"

class HunchBack : public Enemy
{
	const float turningDelay = 1450.f;
	float turnDelayTime = 0;
	bool isOnTurnDelay = false;	
	bool isJumping = false;

public:
	HunchBack();
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL, bool stopMoving = false);
	virtual void Render();
	virtual void SetState(int state);
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void GetActiveBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void LoseHP(int x);
};

