#pragma once
#include "GameObject.h"
#include "PieceWall.h"

class BreakWall : public GameObject
{
	bool isBreakToPieces = false;
	WallPieces* pieces = NULL;

public:
	BreakWall();
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL, bool stopMoving = false);
	void Render();
	void SetState(int state);

	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
};

