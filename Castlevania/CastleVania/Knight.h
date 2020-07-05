#pragma once
#include "Enemy.h"
#include "Ground.h"
#define KNIGHT_ACTIVE			0
#define KNIGHT_DESTROYED		1
#define KNIGHT_INACTIVE			2


class Knight : public Enemy
{
	int left; 
	int right;
public:
	Knight();

	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL);
	virtual void Render();
	virtual void SetState(int state);

	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void GetActiveBoundingBox(float& left, float& top, float& right, float& bottom);

	virtual void LoseHP(int x);

	void SetLeft(int l);
	void SetRight(int r);

};

