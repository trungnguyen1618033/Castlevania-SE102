#pragma once
#include "GameObject.h"

class Zombie : public GameObject
{
	DWORD respawnTime_Start = 0;
	bool isRespawnWaiting = false;

public:
	Zombie();
	~Zombie() {};
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL);
	virtual void Render();
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	void SetState(int state);

	void StartRespawnTimeCounter() { isRespawnWaiting = true; respawnTime_Start = GetTickCount(); }

	void GetActiveBoundingBox(float& left, float& top, float& right, float& bottom);

	void SetIsRespawnWaiting(bool x) { isRespawnWaiting = x; }
	bool IsRespawnWaiting() { return isRespawnWaiting; }
	bool IsAbleToActivate();

};
