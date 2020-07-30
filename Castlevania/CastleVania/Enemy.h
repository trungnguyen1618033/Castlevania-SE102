#pragma once
#include "GameObject.h"
class Enemy : public GameObject
{
protected:
	int hp;
	int score;
	int attack;

	DWORD respawnTime_Start;
	bool isRespawnWaiting;
	int respawnWaitingTime;

	D3DXVECTOR2 entryPosition;

	D3DXVECTOR2 simonPostion;
public:
	bool isSettedPosition;

	Enemy();
	~Enemy();

	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObject = NULL, bool stopMoving = false);
	virtual void Render() = 0;
	virtual void SetState(int state);

	virtual void LoseHP(int x);

	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom) = 0;
	virtual void GetActiveBoundingBox(float& left, float& top, float& right, float& bottom) = 0;

	void RenderActiveBoundingBox();

	void StartRespawnTimeCounter();
	bool IsAbleToActivate();

	void SetEntryPosition(float x, float y) { entryPosition.x = x; entryPosition.y = y; }
	D3DXVECTOR2 GetEntryPosition() { return entryPosition; }
	void SetSimonPosition(float sx, float sy) { simonPostion.x = sx; simonPostion.y = sy; }

	int GetHP() { return hp; }
	int GetScore() { return score; }
	int GetAttack() { return attack; }
};

