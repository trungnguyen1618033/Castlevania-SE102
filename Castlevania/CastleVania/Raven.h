#pragma once
#include "Enemy.h"

#define RAVEN_ACTIVE			0
#define RAVEN_DESTROYED			1
#define RAVEN_INACTIVE			3
#define RAVEN_IDLE				2

class Raven : public Enemy
{
public:
	Raven();

	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL, bool stopMoving = false);
	virtual void Render();
	virtual void SetState(int state);

	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void GetActiveBoundingBox(float& left, float& top, float& right, float& bottom);

	virtual void LoseHP(int x);

};

