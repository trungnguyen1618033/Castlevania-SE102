#pragma once
#include "GameObject.h"
class PieceWall : public GameObject
{
public:
	PieceWall();

	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL);
	void Render();

	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom) {}
};


typedef PieceWall* LPPIECEWALL;


// Mỗi cụm có 4 piece wall
class WallPieces
{
	DWORD startTime = 0;
	vector<LPPIECEWALL> pieces;

public:
	WallPieces(float x, float y);
	~WallPieces();

	void Update(DWORD dt);
	void Render();

	DWORD GetStartTimeRender() { return startTime; }
	void SetStartTimeRender(DWORD x) { startTime = x; }
};
