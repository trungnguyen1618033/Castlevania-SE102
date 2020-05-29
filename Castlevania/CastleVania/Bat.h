#pragma once
#include "GameObject.h"
#define BAT_FLYING_SPEED_X	0.12f
#define BAT_FLYING_SPEED_Y	0.1f
#define BAT_SPEED_VARIATION 0.004f
#define BAT_RESPAWN_TIME	5000

class Bat : public GameObject
{
	DWORD respawnTime_Start = 0;
	bool isRespawnWaiting = false;
	float velocityVariation = BAT_SPEED_VARIATION;
	bool isSettedPosition = false;

public:
	Bat();
	~Bat() {};
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL);
	virtual void Render();
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);

	void SetState(int state);
	void StartRespawnTimeCounter() { isRespawnWaiting = true; respawnTime_Start = GetTickCount(); }
	void GetActiveBoundingBox(float& left, float& top, float& right, float& bottom);

	bool IsRespawnWaiting() { return isRespawnWaiting; }
	void SetIsRespawnWaiting(bool x) { isRespawnWaiting = x; }

	bool IsSettedPosition() { return isSettedPosition; }
	void SetIsSettedPosition(bool x) { isSettedPosition = x; }

	bool IsAbleToActivate();

	DWORD get() { return this->respawnTime_Start; }
};

